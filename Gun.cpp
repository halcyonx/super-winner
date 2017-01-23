#include "stdafx.h"
#include "Gun.h"

class Gun::Self {
public:
	Render::Texture* _tex;
	float _angle;
	FPoint _pos;
};

std::unique_ptr<Gun> Gun::create(Render::Texture* tex, const FPoint& pos) {
	return std::make_unique<Gun>(tex, pos);
}

Gun::Gun(Render::Texture* tex, const FPoint& pos) {
	self = std::unique_ptr<Self>(new Self);
	self->_tex = tex;
	self->_pos = pos;
	self->_angle = 0.f;
}

Gun::~Gun() 
{}

void Gun::Draw()
{
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(self->_pos.x, self->_pos.y - 100, 0);
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), self->_angle);
	Render::device.MatrixTranslate(-self->_tex->_rect_width * 0.5f, -10, 0);
	self->_tex->Draw();
	Render::device.PopMatrix();
}

void Gun::Rotate(float angle)
{
	self->_angle = angle;
}