#include "stdafx.h"
#include "Scores.h"

class Scores::Self {
public:
	int _scores;
	FPoint _pos;
};

std::unique_ptr<Scores> Scores::create(int initial, const FPoint& pos) {
	return std::make_unique<Scores>(initial, pos);
}

Scores::Scores(int initial, const FPoint& pos) {
	self = new Self;
	self->_scores = initial;
	self->_pos = pos;
}

Scores::~Scores() {
	delete self;
}

void Scores::Set(int score) {
	self->_scores = score;
}

int Scores::GetScore() const {
	return self->_scores;
}

void Scores::Draw() {
	Render::BindFont("kids");
	Render::PrintString(self->_pos, "Scores: ", 0.6f, RightAlign);
	Render::PrintString(self->_pos + FPoint(30, 0), std::to_string(static_cast<int>(self->_scores)), 0.6f, CenterAlign);
}