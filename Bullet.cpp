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

	// используются для определния начальной позиции пули при выстреле
	static float ANGLE_X;
	static float ANGLE_Y;

	// гравитация, действующая на пулю
	static float GRAVITY;

	// прямоугольник, используемый при вычислении столкновений усекается на (MARGIN * 100)% с краев
	static float MARGIN;
};
float Bullet::Self::ANGLE_X = -175.f;
float Bullet::Self::ANGLE_Y = 200.f;
float Bullet::Self::GRAVITY = 0.045f;
float Bullet::Self::MARGIN = .1f;

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
	self->_delta.y -= self->GRAVITY;
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

void Bullet::InitDirection(const IPoint& mouse_pos) {
	if (!self->_fly) {
		self->_fly = true;
		auto angle = atan2(mouse_pos.y, Render::device.Width() * 0.5f - mouse_pos.x);
		self->_sp = self->_st + FPoint(self->ANGLE_X * cos(angle), self->ANGLE_Y * sin(angle));
		self->_delta = self->_sp - mouse_pos;
		auto th = atan((self->_delta.y) / (self->_delta.x));
		self->_delta.x = self->_speed * cos(th);
		self->_delta.y = self->_speed * sin(th);
		if (mouse_pos.x < self->_pos.x)
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
	auto w = rect.Width() * self->MARGIN; 
	auto h = rect.Height() * self->MARGIN;
	self->_rect = IRect(self->_pos.x + w, self->_pos.y + h, rect.RightTop().x - w, rect.RightTop().y - h);
	return self->_rect;
}

const FPoint& Bullet::GetPos() const {
	return self->_pos;
}

std::unique_ptr<Effect> Bullet::effect() {
	return move(self->_effect);
}