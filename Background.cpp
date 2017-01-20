#include "stdafx.h"
#include "Background.h"

std::unique_ptr<Background> Background::create(Render::Texture* tex) {
	return std::make_unique<Background>(tex);

}
class Background::Self {
public:
	Render::Texture* _tex;
};

Background::Background(Render::Texture* tex) {
	self = new Self;
	self->_tex = tex;
}

Background::~Background() {
	delete self;
}

void Background::Draw() {
	Render::device.PushMatrix();
	self->_tex->Draw();
	Render::device.PopMatrix();
}