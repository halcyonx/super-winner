﻿#include "stdafx.h"
#include "Timer.h"

class Timer::Self {
public:
	float _t;
	float _acc;
	float _prev;
	float _init;
	bool _active;
	bool _reverse;
	FPoint _pos;
	float _scale;
};

std::unique_ptr<Timer> Timer::create(int t, const FPoint& pos, bool reverse) {
	return std::make_unique<Timer>(t, pos, reverse);
}

Timer::Timer(int t, const FPoint& pos, bool reverse) {
	self = std::unique_ptr<Self>(new Self);
	self->_t = (reverse ? .0f : t);
	self->_acc = (reverse ? t : .0f);
	self->_init = self->_acc;
	self->_prev = self->_acc;
	self->_active = false;
	self->_pos = pos;
	self->_reverse = reverse;
	self->_scale = 1.f;
}

Timer::~Timer() 
{}

void Timer::Update(float dt) {
	if (self->_active) {
		if (!self->_reverse)
			self->_acc += dt;
		else
			self->_acc -= dt;

		if (abs(self->_t - self->_acc) <= 4) {
			if (self->_scale <= 1.f || (static_cast<int>(self->_prev) != static_cast<int>(self->_acc))) {
				self->_scale = 2.f;
				self->_prev = self->_acc;
			}
			float d = -dt*0.7f;
			self->_scale += d;
		}
	}
}

void Timer::Start() {
	self->_active = true;
}

void Timer::Stop() {
	self->_active = false;
}

bool Timer::Expired() const {
	if (!self->_reverse) {
		if (self->_acc >= self->_t) {
			self->_active = false;
			return true;
		}
	}
	else {
		if (self->_acc <= 0) {
			self->_active = false;
			return true;
		}
	}
	return false;
}

bool Timer::IsActive() const {
	return self->_active;
}

void Timer::Reset() {
	self->_active = false;
	self->_scale = 1.f;
	self->_acc = self->_init;
}

void Timer::Draw() {
	Render::BindFont("kids");
	Render::PrintString(self->_pos, std::to_string(static_cast<int>(self->_acc)), self->_scale, CenterAlign);
}