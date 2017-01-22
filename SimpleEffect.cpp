#include "stdafx.h"
#include "SimpleEffect.h"

std::unique_ptr<SimpleEffect> SimpleEffect::create(const FPoint& pos) {
	return std::make_unique<SimpleEffect>(pos);
}

class SimpleEffect::Self {
public:
	EffectsContainer _effCont;
	ParticleEffectPtr _eff;
	FPoint _pos;
};

SimpleEffect::SimpleEffect(const FPoint& pos) {
	self = new Self;
	self->_pos = pos;
	self->_eff = self->_effCont.AddEffect("Smoke1");
	self->_eff->posX = self->_pos.x;
	self->_eff->posY = self->_pos.y;
	self->_eff->Reset();
}

SimpleEffect::~SimpleEffect() {
	delete self;
	self = nullptr;
}

void SimpleEffect::Draw() {
	self->_effCont.Draw();
}

void SimpleEffect::SetPos(const FPoint& pos) {
	self->_pos = pos;
}

void SimpleEffect::Update(float dt) {
	if (self->_eff) {
		self->_eff->posX = self->_pos.x + 0.f;
		self->_eff->posY = self->_pos.y + 0.f;
		self->_effCont.Update(dt);
	}
}