#include "stdafx.h"
#include "Target.h"

class Target::Self {
public:
	Render::Texture* _tex;
	FPoint _pos;
	FPoint _dir;
	IRect _rect;
	int _width;
	int _height;
	float _scale;
	bool _killed;

	// граница снизу, чтобы мишени не залетали на пушку
	static float BOTTOM_MARGIN;

	// прямоугольник, используемый при вычислении столкновений усекается на (MARGIN * 100)% с краев
	static float MARGIN;
};
float Target::Self::BOTTOM_MARGIN = 200.f;
float Target::Self::MARGIN = .05f;

std::unique_ptr<Target> Target::create(Render::Texture* tex, const FPoint& pos, const FPoint& dir, float scale) {
	return std::make_unique<Target>(tex, pos, dir, scale);
}

Target::Target(Render::Texture* tex, const FPoint& pos, const FPoint& dir, float scale) {
	self = std::unique_ptr<Self>(new Self);
	self->_tex = tex;
	self->_pos = pos;
	self->_dir = dir;
	self->_scale = scale;
	self->_height = Render::device.Height();
	self->_width = Render::device.Width();
	self->_killed = false;
}

Target::~Target() 
{}

void Target::Draw() {
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(self->_pos);
	Render::device.MatrixScale(self->_scale, self->_scale, 0.0);
	Render::device.MatrixTranslate(-self->_tex->_rect_width * 0.5f, -self->_tex->_rect_height * 0.5f, 0);
	self->_tex->Draw();
	Render::device.PopMatrix();
}

void Target::Update(float dt)
{
	self->_pos.x += self->_dir.x;
	self->_pos.y += self->_dir.y;
	auto w = self->_tex->_rect_width * self->_scale * 0.5f;
	if (self->_pos.y >= self->_height - w || self->_pos.y <= w + self->BOTTOM_MARGIN) {
		self->_dir.y = -self->_dir.y;
	}
	if (self->_pos.x >= self->_width - w || self->_pos.x <= w) {
		self->_dir.x = -self->_dir.x;
	}
}

void Target::SetScale(float scale)
{
	self->_scale = scale;
}

void Target::SetDirection(const FPoint& dir)
{
	self->_dir = dir;
}

void Target::Kill() {
	self->_killed = true;
}

bool Target::IsKilled() const {
	return self->_killed;
}

const IRect& Target::GetRect() const
{
	auto rect = self->_tex->getBitmapRect();
	auto w = rect.Width() * self->MARGIN;
	auto h = rect.Height() * self->MARGIN;
	self->_rect = IRect(self->_pos.x, self->_pos.y, rect.RightTop().x * self->_scale - w, rect.RightTop().y * self->_scale - h);
	return self->_rect;
}