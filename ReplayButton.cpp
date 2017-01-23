#include "stdafx.h"
#include "ReplayButton.h"

class ReplayButton::Self {
public:
	Render::Texture* _tex;
	Render::Texture* _replay_normal;
	Render::Texture* _replay_active;
	Render::Texture* _replay_pressed;
	bool _visible;
	FPoint _pos;
	IRect _rect;
};

std::unique_ptr<ReplayButton> ReplayButton::create(
	Render::Texture *normal,
	Render::Texture *active,
	Render::Texture *pressed,
	const FPoint& pos) {

	return std::make_unique<ReplayButton>(normal, active, pressed, pos);
}

ReplayButton::ReplayButton(Render::Texture *normal, Render::Texture *active, Render::Texture *pressed, const FPoint& pos) {
	self = std::unique_ptr<Self>(new Self);
	self->_tex = normal;
	self->_replay_normal = normal;
	self->_replay_active = active;
	self->_replay_pressed = pressed;
	self->_pos = pos;
	self->_visible = false;
}

ReplayButton::~ReplayButton() 
{}

void ReplayButton::Draw() {
	if (self->_visible) {
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(self->_pos);
		Render::device.MatrixTranslate(-self->_tex->_rect_width * 0.5f, -self->_tex->_rect_height * 0.5f, 0);
		self->_tex->Draw();
		Render::device.PopMatrix();
	}
}

void ReplayButton::Show() {
	self->_visible = true;
}

void ReplayButton::Hide() {
	self->_visible = false;
}

bool ReplayButton::MouseOn(const IPoint& mouse_pos) {
	if (!self->_visible) return false;
	auto rect = self->_tex->getBitmapRect();
	self->_rect = IRect(self->_pos.x - rect.width * 0.5f, self->_pos.y - rect.height * 0.5f, rect.width, rect.height);
	if (self->_rect.Contains(mouse_pos))
		return true;
	return false;
}

void ReplayButton::Update(float dt) {

}

void ReplayButton::MouseMove(const IPoint& mouse_pos) {
	if (MouseOn(mouse_pos))
		self->_tex = self->_replay_active;
	else
		self->_tex = self->_replay_normal;
}

bool ReplayButton::MouseDown(const IPoint& mouse_pos) {
	if (MouseOn(mouse_pos)) {
		self->_tex = self->_replay_pressed;
		return true;
	}
	return false;
}

bool ReplayButton::MouseUp(const IPoint& mouse_pos) {
	if (MouseOn(mouse_pos)) {
		self->_tex = self->_replay_normal;
		return true;
	}
	return false;
}