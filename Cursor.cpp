#include "stdafx.h"
#include "Cursor.h"

class Cursor::Self {
public:
	Render::Texture* _tex;
};

std::unique_ptr<Cursor> Cursor::create(Render::Texture* tex) {
	return std::make_unique<Cursor>(tex);
}

Cursor::Cursor(Render::Texture* tex) {
	self = std::unique_ptr<Self>(new Self);
	self->_tex = tex;
}

Cursor::~Cursor() 
{}

void Cursor::Draw() {
	Render::device.PushMatrix();
	FPoint p{ Core::mainInput.GetMousePos() };
	Render::device.MatrixTranslate(p);
	Render::device.MatrixTranslate(-self->_tex->_rect_width * 0.5f, -self->_tex->_rect_height * 0.5f, 0);
	self->_tex->Draw();
	Render::device.PopMatrix();
}