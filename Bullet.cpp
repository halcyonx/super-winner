#include "stdafx.h"
#include "Bullet.h"

class Bullet::Self {
public:
	Render::Texture* _tex;
	std::unique_ptr<Effect> _effect;
	IRect _rect;
	FPoint _pos;
	FPoint _st, _sp;
	FPoint _delta;
	bool _fly;
	float _angle;
	int _speed;
	float _gravity;
};

std::unique_ptr<Bullet> Bullet::create(Render::Texture* tex, const FPoint& pos, int speed) {
	return std::make_unique<Bullet>(tex, pos, speed);
}

Bullet::Bullet(Render::Texture* tex, const FPoint& pos, int speed) {
	self = std::unique_ptr<Self>(new Self);
	
	self->_tex = tex;
	self->_pos = pos;
	self->_st = pos;
	self->_speed = speed;
	self->_fly = false;
	self->_gravity = 0.045;
	self->_angle = .0f;
}

Bullet::~Bullet() 
{}

void Bullet::Draw() {
	if (self->_fly)
	{
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(self->_pos.x, self->_pos.y, 0);
		Render::device.MatrixRotate(math::Vector3(0, 0, 1), self->_angle);
		Render::device.MatrixTranslate(-self->_tex->_rect_width * 0.5f, -self->_tex->_rect_height * 0.5f, 0);
		self->_tex->Draw();
		Render::device.PopMatrix();
		self->_effect->Draw();
	}
}

void Bullet::Update(float dt)
{
	self->_pos += self->_delta;
	self->_delta.y -= self->_gravity;
	self->_angle += -10.;
	auto r = FPoint(sin(self->_angle)*7.f, cos(self->_angle)*7.f);
	self->_effect->SetPos(self->_pos + r);
	self->_effect->Update(dt);
	if (self->_pos.x >= Render::device.Width() || self->_pos.x <= 0)
	{
		self->_fly = false;
	}
	if (self->_pos.y >= Render::device.Height() || self->_pos.y <= 0)
	{
		self->_fly = false;
	}
}

void Bullet::SetStartPoint(const IPoint& mouse_pos) {
	if (!self->_fly) {
		float ex = Render::device.Width() * 0.5f;
		float dx = ex - mouse_pos.x;
		float dy = mouse_pos.y;
		auto angle = atan2(dy, dx);
		auto x = -175 * cos(angle);
		auto y = 200 * sin(angle);
		self->_pos = self->_st + FPoint(x, y);
		self->_sp = self->_pos;
	}
}

void Bullet::FlyTo(const FPoint& p) {
	if (!self->_fly) {
		self->_fly = true;
		self->_pos = self->_sp;
		self->_delta = self->_pos - p;
		auto th = atan((self->_delta.y) / (self->_delta.x));
		self->_delta.x = self->_speed * cos(th);
		self->_delta.y = self->_speed * sin(th);
		if (p.x < self->_pos.x)
		{
			self->_delta.x = -self->_delta.x;
			self->_delta.y = -self->_delta.y;
		}
		self->_pos = self->_sp + 5 * self->_delta;
		self->_effect = Effect::create("TestedFire", self->_pos, false);
	}
}

bool Bullet::IsFly() const {
	return self->_fly;
}

void Bullet::Stop() const {
	self->_fly = false;
}

const IRect& Bullet::GetRect() const {
	auto rect = self->_tex->getBitmapRect();
	auto w = rect.Width() * .1f;
	auto h = rect.Height() * .1f;
	self->_rect = IRect(self->_pos.x + w, self->_pos.y + h, rect.RightTop().x - w, rect.RightTop().y - h);
	return self->_rect;
}

const FPoint& Bullet::GetPos() const {
	return self->_pos;
}

std::unique_ptr<Effect> Bullet::effect() {
	return move(self->_effect);
}