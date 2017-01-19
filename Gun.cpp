#include "stdafx.h"
#include "Gun.h"

class Gun::Self {
public:
	Render::Texture* _tex;
	float _angle;
	math::Vector3 _pos;
};

std::shared_ptr<Gun> Gun::create(Render::Texture* tex, const math::Vector3& pos) {
	return std::make_shared<Gun>(tex, pos);
}

Gun::Gun(Render::Texture* tex, const math::Vector3& pos) {
	self = new Self;
	self->_tex = tex;
	self->_pos = pos;
	self->_angle = 90.f;
}

Gun::~Gun() {
	delete self;
}

void Gun::Draw()
{
	Render::device.PushMatrix();
	Render::device.MatrixTranslate(self->_pos.x, self->_pos.y - 100, 0);
	Render::device.MatrixRotate(math::Vector3(0, 0, 1), self->_angle);
	Render::device.MatrixTranslate(-self->_tex->_rect_width * 0.5f, -10, 0);
	self->_tex->Draw();
	Render::device.PopMatrix();
}

void Gun::Update(float dt)
{

}

void Gun::Shoot(float x, float y)
{

}

void Gun::Rotate(float angle)
{
	self->_angle = angle;
}