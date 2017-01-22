#include "stdafx.h"
#include "Shooter.h"
#include "Background.h"
#include "Target.h"
#include "Gun.h"
#include "Bullet.h"
#include "Effect.h"
#include "GameUtils.h"
#include "Config.h"
#include <memory>

#define TESTED
#define DEBUG

#define ptr(t) \
std::unique_ptr<t>

using Targets = std::list<std::unique_ptr<Target>>;
using Bullets = std::list<std::unique_ptr<Bullet>>;
using Effects = std::list<std::unique_ptr<Effect>>;

class Shooter::Self {
public:
	Self()
		: _timer(0)
		, _acc(0)
		, _run(true)
	{}

	// ���������� ����, ������� ����� ���� �������� ������������
	const unsigned int MAX_BULLETS = 3;
	const float SHOOT_DELAY = .5f;
	bool _run;
	float _timer, _acc;

	ptr(Background) _background;
	ptr(Gun) _gun;
	Targets _targets;
	Bullets _bullets;
	Effects _effects;

	// ���� ������ �� ������� ������ � ���� ���� � ������ �������� � ����������� ��������, ����� ��������
	bool is_allow_to_shoot(const IPoint& mouse_pos) {
		if (mouse_pos.y > 250 && _acc > SHOOT_DELAY)
			return true;
		return false;
	}
	
	// �������� ������������ ���� � ��������
	void collide() {
		for (auto &bullet : _bullets) {
			auto bullet_rect = bullet->GetRect();
			for (auto iter = _targets.begin(); iter != _targets.end(); ++iter)
				if (bullet_rect.Intersects((*iter)->GetRect())) {
					//auto temp = iter++;
					_targets.erase(iter);
					bullet->Stop();
					_effects.push_back(Effect::create(bullet->GetPos()));
					_effects.back()->Start();
					break;
				}
		}
	}

	// ������������� �������
	void init_targets() {
		_targets.clear();
		for (int i = 0; i < Config::get("CountTarget"); ++i) {
			_targets.push_back(
				Target::create(
					Core::resourceManager.Get<Render::Texture>("blue_bubble"),
					game_utils::random_vec(300, 600), // ��������� ��������� �������
					game_utils::random_vec(-5, 5), // ��������� �����������
					game_utils::random_float() // ��������� ������
				)
			);
		}
	}
};

Shooter::Shooter(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
{
	self = new Self;
	Init();
}

Shooter::~Shooter() {
	delete self;
}

void Shooter::Init()
{
	self->_background = Background::create(Core::resourceManager.Get<Render::Texture>("background01"));
	self->_gun = Gun::create(Core::resourceManager.Get<Render::Texture>("gun"), FPoint(Render::device.Width() * 0.5f, 80.f));

	// ������������� �������
	self->init_targets();
}

void Shooter::Reset() {
	self->_bullets.clear();
	self->_effects.clear();
	self->init_targets();
}

void Shooter::Draw()
{
	// draw stuff
	self->_background->Draw();

	for (auto& target : self->_targets) {
		target->Draw();
	}

	for (auto& bullet : self->_bullets)
		bullet->Draw();

	self->_gun->Draw();

	for (auto &effect : self->_effects) {
		effect->Draw();
	}

#ifdef DEBUG
	IPoint mouse_pos = Core::mainInput.GetMousePos();
	Render::BindFont("arial");
	Render::PrintString(924 + 100 / 2, 35, utils::lexical_cast(mouse_pos.x) + ", " + utils::lexical_cast(mouse_pos.y), 1.f, CenterAlign);
#endif // DEBUG
}

void Shooter::Update(float dt)
{
	if (!self->_run) return;
	self->_timer += dt;
	self->_acc += dt;

	while (self->_timer > 2 * math::PI)
	{
		self->_timer -= 2 * math::PI;
	}
	if (self->_acc >= 10.f)
		self->_acc = self->SHOOT_DELAY;

	for (auto& target : self->_targets) {
		target->Update(dt);
	}

	for (auto iter = self->_bullets.begin(); iter != self->_bullets.end();) {
		(*iter)->Update(dt);
		auto temp = iter++;
		if (!(*temp)->IsFly())
			self->_bullets.erase(temp);
	}

	self->collide();

	for (auto iter = self->_effects.begin(); iter != self->_effects.end(); ) {
		(*iter)->Update(dt);
		if ((*iter)->Expires() && (*iter)->Alive()) {
			(*iter)->Alive(false);
			Core::guiManager.getLayer("TestLayer")->getWidget("Interface")->AcceptMessage(Message("", "ScoreAdd"));
		}
		if (!(*iter)->IsActive()) {
			iter = self->_effects.erase(iter);
			continue;
		}
		iter++;
	}
}

bool Shooter::MouseDown(const IPoint &mouse_pos)
{
	if (!self->_run) return false;
	if (Core::mainInput.GetMouseRightButton())
	{
	}
	else 
	{
		if (self->_bullets.size() < self->MAX_BULLETS && self->is_allow_to_shoot(mouse_pos)) {
			self->_bullets.push_back(Bullet::create(Core::resourceManager.Get<Render::Texture>("cannonball"),
				FPoint(Render::device.Width() * 0.5f, -30), Config::get("Speed")));
			self->_bullets.back()->SetStartPoint(mouse_pos);
			self->_bullets.back()->FlyTo(FPoint(mouse_pos.x, mouse_pos.y));
			self->_acc = .0f;
		}
	}
	return false;
}

void Shooter::MouseMove(const IPoint &mouse_pos)
{
	if (!self->_run) return;
	// �� ������������ ����� �� ����
	if (mouse_pos.y <= 250) {
		return;
	}
	// �������� ���� ������� �����, ������������ �� ��������� �������
	float ex = Render::device.Width() * 0.5f;
	float dx = ex - mouse_pos.x;
	float dy = mouse_pos.y;
	auto th = atan2(dx, dy);
	th *= 180 / math::PI;
	// ������������ �����
	self->_gun->Rotate(th);
}

void Shooter::MouseUp(const IPoint &mouse_pos)
{
}

void Shooter::AcceptMessage(const Message& message)
{
	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
	if (data == "StopGame") {
		self->_run = false;
	}
	else if (data == "RestartGame") {
		Reset();
		self->_run = true;
	}
}

void Shooter::KeyPressed(int keyCode)
{
	if (keyCode == VK_ESCAPE) {
		Core::appInstance->CloseWindow();
	}
}

void Shooter::CharPressed(int unicodeChar)
{
}
