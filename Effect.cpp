#include "stdafx.h"
#include "Effect.h"

std::unique_ptr<Effect> Effect::create(const FPoint& pos) {
	return std::make_unique<Effect>(pos);
}

class Effect::Self {
public:
	EffectsContainer _effCont;
	ParticleEffectPtr _eff;
	TimedSpline<FPoint> _spline;
	std::vector<FPoint> _coords;
	int _i;
	int _size;
	float _dt;
	float _t;
	bool _active;
	bool _alive;
};

Effect::Effect(const FPoint& pos) {
	self = new Self;
	self->_i = 0;
	self->_dt = .0f;
	self->_active = false;
	self->_alive = false;
	auto r = (rand() % 2) ? true : false;
	self->_coords = std::vector<FPoint>{
		FPoint(pos),
		FPoint((r) ? FPoint(615.f, 230.f) : FPoint(885.f, 370.f)),
		/*FPoint(675.f, 280.f),
		FPoint(885.f, 370.f),
		FPoint(935.f, 302.f),*/ 
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

Effect::~Effect() {
	delete self;
	self = nullptr;
	Log::Debug("Effect killed");
}

void Effect::Draw() {
	self->_effCont.Draw();
	if (self->_dt >= self->_t) {
		Stop();
	}
}

void Effect::Update(float dt) {
	if (self->_eff) {
		FPoint currentPosition = self->_spline.getGlobalFrame(self->_dt);
		self->_eff->posX = currentPosition.x + 0.f;
		self->_eff->posY = currentPosition.y + 0.f;
		self->_effCont.Update(dt);
		self->_dt += dt / 3.f;
	}
}

void Effect::Start() {
	self->_active = true;
	self->_alive = true;
	self->_eff = self->_effCont.AddEffect("Iskra2");
	self->_eff->posX = self->_coords[0].x + 0.f;
	self->_eff->posY = self->_coords[0].y + 0.f;
	self->_eff->Reset();
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