#include "stdafx.h"
#include "Effect.h"

std::unique_ptr<Effect> Effect::create(const std::string& name, const FPoint& pos, bool splined) {
	return std::make_unique<Effect>(name, pos, splined);
}

class Effect::Self {
public:
	EffectsContainer _effCont;
	ParticleEffectPtr _eff;
	TimedSpline<FPoint> _spline;
	std::vector<FPoint> _coords;
	FPoint _pos;
	float _dt;
	float _t;
	bool _active;
	bool _alive;
	bool _splined;
};

Effect::Effect(const std::string& name, const FPoint& pos, bool splined) {
	self = std::unique_ptr<Self>(new Self);
	self->_dt = .0f;
	self->_active = false;
	self->_alive = false;
	self->_splined = splined;
	self->_eff = self->_effCont.AddEffect(name);
	self->_pos = pos;
	self->_eff->posX = pos.x;
	self->_eff->posY = pos.y;
	self->_eff->Reset();
	if (self->_splined) {
		auto r = (rand() % 2) ? true : false;
		self->_coords = std::vector<FPoint>{
			FPoint(pos),
			FPoint((r) ? FPoint(615.f, 230.f) : FPoint(885.f, 370.f)),
			FPoint(949.f, 118.f)
		};

		float d = 0.f;
		for (auto &coord : self->_coords) {
			self->_spline.addKey(d, coord);
			d += 0.25f;
		}
		self->_t = d - 0.25f;
		self->_spline.CalculateGradient();
	}
}

Effect::~Effect() 
{}

void Effect::Draw() {
	self->_effCont.Draw();
	if (self->_splined && self->_dt >= self->_t) {
		Stop();
	}
}

void Effect::Update(float dt) {
	if (self->_eff) {
		FPoint currentPosition = self->_splined ? self->_spline.getGlobalFrame(self->_dt) : self->_pos;
		self->_eff->posX = currentPosition.x + 0.f;
		self->_eff->posY = currentPosition.y + 0.f;
		self->_effCont.Update(dt);
		self->_dt += dt / 3.f;
	}
}

void Effect::Start() {
	self->_active = true;
	self->_alive = true;
	self->_eff->posX = self->_coords[0].x + 0.f;
	self->_eff->posY = self->_coords[0].y + 0.f;
}

void Effect::SetPos(const FPoint& pos) {
	self->_pos = pos;
}

void Effect::Stop() {
	self->_active = false;
	self->_eff->Finish();
}

bool Effect::IsActive() const {
	return !self->_eff->isEnd();
}

bool Effect::Expires() const {
	return self->_dt > self->_t;
}

bool Effect::Alive(bool a) {
	if (a)
		return self->_alive;
	self->_alive = a;
	return self->_alive;
}