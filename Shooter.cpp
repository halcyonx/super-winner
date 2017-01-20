#include "stdafx.h"
#include "Shooter.h"
#include "Background.h"
#include "Target.h"
#include "Gun.h"
#include "Bullet.h"
#include "GameUtils.h"
#include "Config.h"
#include <memory>

#define TESTED
#define DEBUG

#define type(t) \
std::shared_ptr<t>

class Shooter::Self {
public:
	Self()
		: _timer(0)
		, _acc(0)
		, _run(true)
	{}

	// количество пуль, которые могут быть выпущены одновременно
	const unsigned int MAX_BULLETS = 3;
	const float SHOOT_DELAY = .5f;
	bool _run;

	// _params служит для хранения параметров игры (CountTarget, Speed, Time)
	std::map<std::string, int> _params;

	std::unique_ptr<Background> _background;
	std::unique_ptr<Gun> _gun;
	std::list<std::unique_ptr<Target>> _targets;
	std::list<std::unique_ptr<Bullet>> _bullets;
	float _timer, _acc;

	// если курсор не слишком близко к краю окна и прошла задержка с предыдущего выстрела, можно стрелять
	bool is_allow_to_shoot(const IPoint& mouse_pos) {
		if (mouse_pos.y > 250 && _acc > SHOOT_DELAY)
			return true;
		return false;
	}
	
	// проверка столкновения пули с мишенями
	void collide() {
		for (auto &bullet : _bullets) {
			auto bullet_rect = bullet->GetRect();
			for (auto iter = _targets.begin(); iter != _targets.end(); ++iter)
				if (bullet_rect.Intersects((*iter)->GetRect())) {
					auto temp = iter++;
					_targets.erase(temp);
					bullet->Stop();
					Core::guiManager.getLayer("TestLayer")->getWidget("Interface")->AcceptMessage(Message("", "ScoreAdd"));
					break;
				}
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
	self->_gun = Gun::create(Core::resourceManager.Get<Render::Texture>("gun"), math::Vector3(Render::device.Width() * 0.5f, 80, 0));

	// инициализация мишеней
	for (int i = 0; i < Config::get("CountTarget"); ++i) {
		self->_targets.push_back(
			Target::create(
				Core::resourceManager.Get<Render::Texture>("blue_bubble"),
				game_utils::random_vec(300, 600), // случайная стартовая позиция
				game_utils::random_vec(-5, 5), // случайное направление
				game_utils::random_float() // случайный размер
			)
		);
	}
}

void Shooter::Reset() {
	self->_bullets.clear();
	self->_targets.clear();
	for (int i = 0; i < Config::get("CountTarget"); ++i) {
		self->_targets.push_back(
			Target::create(
				Core::resourceManager.Get<Render::Texture>("blue_bubble"),
				game_utils::random_vec(300, 600), // случайная стартовая позиция
				game_utils::random_vec(-5, 5), // случайное направление
				game_utils::random_float() // случайный размер
			)
		);
	}
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
	// не поворачиваем пушку на себя
	if (mouse_pos.y <= 250) {
		return;
	}
	// получаем угол наклона пушки, взависимости от положения курсора
	float ex = Render::device.Width() * 0.5f;
	float dx = ex - mouse_pos.x;
	float dy = mouse_pos.y;
	auto th = atan2(dx, dy);
	th *= 180 / math::PI;
	// поворачиваем пушку
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
