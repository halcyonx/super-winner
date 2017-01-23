#include "stdafx.h"
#include "TestWidget.h"

// #define TESTED

#ifdef TESTED

#include "Effect.h"

using Effects = std::list<std::unique_ptr<Effect>>;

#endif // TESTED



class TestWidget::Self {
public:
#ifdef TESTED
	Render::Texture* _test_gun;
	std::unique_ptr<Distortion> _d;
	Effects _effects;

	EffectsContainer _effCont;
	ParticleEffectPtr _eff;
	TimedSpline<FPoint> _spline;
	std::vector<FPoint> _coords;
	float _timer;
#endif // TESTED
};

TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
{
#ifdef TESTED
	self = new Self;
	self->_test_gun = Core::resourceManager.Get<Render::Texture>("gun");
	self->_d = std::make_unique<Distortion>(2, 5);
	self->_d->SetRenderRect(self->_test_gun->getBitmapRect());
	self->_timer = .0f;
	Init();
#endif // TESTED
}
TestWidget::~TestWidget() {
#ifdef TESTED
	delete self;
#endif // TESTED
}

void TestWidget::Init() {

}

void TestWidget::Draw() {
#ifdef TESTED
	for (auto &effect : self->_effects) {
		effect->Draw();
	}
#endif // DEBUG
}

void TestWidget::Update(float dt) {
#ifdef TESTED
	self->_timer += dt * 2;
	while (self->_timer > 2 * math::PI)
	{
		self->_timer -= 2 * math::PI;
	}

	for (auto iter = self->_effects.begin(); iter != self->_effects.end();) {
		(*iter)->Update(dt);
		if (!(*iter)->IsActive()) {
			auto t = self->_effects.erase(iter);
			iter = t;
			continue;
		}
		++iter;
	}
#endif // TESTED
}

void TestWidget::AcceptMessage(const Message& message) {

}

bool TestWidget::MouseDown(const IPoint& mouse_pos) {
	return false;
}

void TestWidget::MouseMove(const IPoint& mouse_pos) {
	
}

void TestWidget::MouseUp(const IPoint& mouse_pos) {
#ifdef TESTED
	self->_effects.push_back(Effect::create(mouse_pos));
	self->_effects.back()->Start();
#endif // TESTED
}

void TestWidget::KeyPressed(int keyCode) {

}

void TestWidget::CharPressed(int unicodeChar) {

}