#include "stdafx.h"
#include "Bullet.h"

class Bullet::Self {
public:
	Render::Texture* _tex;
	IRect _rect;
	FPoint _pos;
	FPoint _st, _sp;
	FPoint _delta;
	bool _fly;
	float _dx, _dy;
	float _angle;
	int _speed;
};

std::shared_ptr<Bullet> Bullet::create(Render::Texture* tex, const FPoint& pos, int speed) {
	return std::make_shared<Bullet>(tex, pos, speed);
}

Bullet::Bullet(Render::Texture* tex, const FPoint& pos, int speed) {
	self = new Self;
	
	self->_tex = tex;
	self->_pos = pos;
	self->_st = pos;
	self->_speed = speed;
	self->_fly = false;
}

Bullet::~Bullet() {
	delete self;
}

void Bullet::Draw() {
	if (self->_fly)
	{
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(self->_pos.x, self->_pos.y, 0);
		Render::device.MatrixTranslate(-self->_tex->_rect_width * 0.5f, -self->_tex->_rect_height * 0.5f, 0);
		self->_tex->Draw();
		Render::device.PopMatrix();
	}
}

void Bullet::Update(float dt)
{
	self->_pos += self->_delta;
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
		self->_pos = self->_sp + 5 * self->_delta; // math::Vector3(_st.x + _dx * 15, _st.y + _dy * 10, 0);
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