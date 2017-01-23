#include "stdafx.h"
#include "Win.h"

class WinPlate::Self {
public:
	Render::Texture* _tex;
	FPoint _pos;
	bool _show;
};

std::unique_ptr<WinPlate> WinPlate::create(Render::Texture* tex, const FPoint& pos) {
	return std::make_unique<WinPlate>(tex, pos);
}

WinPlate::WinPlate(Render::Texture* tex, const FPoint& pos) {
	self = std::unique_ptr<Self>(new Self);
	self->_tex = tex;
	self->_pos = pos;
	self->_show = false;
}

WinPlate::~WinPlate() 
{}

void WinPlate::Draw() {
	if (self->_show) {
		Render::device.PushMatrix();
		Render::device.MatrixTranslate(self->_pos);
		Render::device.MatrixTranslate(-self->_tex->Width() * 0.5f, 100.f, 0);
		self->_tex->Draw();
		Render::device.PopMatrix();
	}
}

void WinPlate::Show() {
	self->_show = true;
}

void WinPlate::Hide() {
	self->_show = false;
}