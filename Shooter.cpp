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

// #define DEBUG

#define ptr(t) \
std::unique_ptr<t>

using Targets = std::vector<std::unique_ptr<Target>>;
using Bullets = std::vector<std::unique_ptr<Bullet>>;
using Effects = std::vector<std::unique_ptr<Effect>>;

class Shooter::Self {
public:
	bool _run;
	// _acc отсчитывает задержку после выстрела
	float _acc;

	ptr(Background) _background;
	ptr(Gun) _gun;
	Targets _targets;
	Bullets _bullets;
	Effects _effects;

	// точка где, будут воскресать пули
	const FPoint BULLET_RESPAWN_POINT = FPoint(Render::device.Width() * 0.5f, -30.f);
	// граница снизу, чтобы мишени не залетали на пушку
	const float BOTTOM_MARGIN = 250.f;
	// количество пуль, которые могут быть выпущены одновременно
	const unsigned int MAX_BULLETS = 3;
	// задержка прсое выстрела
	const float SHOOT_DELAY = .5f;

public:
	Self()
		: _acc(0)
		, _run(true)
	{}

	// если курсор не слишком близко к краю окна и прошла задержка с предыдущего выстрела, можно стрелять
	bool is_allow_to_shoot(const IPoint& mouse_pos) {
		if (mouse_pos.y > BOTTOM_MARGIN && _acc > SHOOT_DELAY)
			return true;
		return false;
	}
	
	// проверка столкновения пули с мишенями
	void collide() {
		for (auto &bullet : _bullets) {
			auto bullet_rect = bullet->GetRect();
			for (auto iter = _targets.begin(); iter != _targets.end(); ++iter)
				if (bullet_rect.Intersects((*iter)->GetRect())) {
					bullet->Stop();
					(*iter)->Kill();
					_effects.push_back(Effect::create("Iskra2", bullet->GetPos()));
					_effects.back()->Start();
					break;
				}
		}
	}

	// удаляем отработавшие объекты
	void remove() {
		// пули
		auto end1 = std::remove_if(_bullets.begin(), _bullets.end(), [&](const ptr(Bullet)& ptr) {
			if (!ptr->IsFly()) {
				// эффект от пули передаем во владение Shooter, чтобы эффект плавно закончился
				_effects.push_back(ptr->effect());
				_effects.back()->Stop();
			}
			return !ptr->IsFly();
		});
		if (end1 != _bullets.end())
			_bullets.resize(end1 - _bullets.begin());

		// убитые мишени
		auto end2 = std::remove_if(_targets.begin(), _targets.end(), [&](const ptr(Target)& ptr) {
			return ptr->IsKilled();
		});
		if (end2 != _targets.end())
			_targets.resize(end2 - _targets.begin());

		// умирающие эффекты
		auto end3 = std::remove_if(_effects.begin(), _effects.end(), [&](const ptr(Effect)& ptr) {
			if (ptr->Expires() && ptr->Alive()) {
				ptr->Alive(false);
				// когда искра долетит до позииции с очками, увеличить значение
				Core::guiManager.getLayer("TestLayer")->getWidget("Interface")->AcceptMessage(Message("", "ScoreAdd"));
			}
			return !ptr->IsActive();
		});
		if (end3 != _effects.end())
			_effects.resize(end3 - _effects.begin());
	}

	// инициализация мишеней
	void init_targets() {
		_targets.clear();
		for (int i = 0; i < Config::get("CountTarget"); ++i) {
			_targets.push_back(
				Target::create(
					Core::resourceManager.Get<Render::Texture>("blue_bubble"),
					game_utils::random_vec(BOTTOM_MARGIN * 1.5f, Render::device.Height() * .8f), // случайная стартовая позиция
					game_utils::random_vec(-Config::get("Speed"), Config::get("Speed")), // случайное направление
					game_utils::random_float() // случайный размер
				)
			);
		}
	}
};

Shooter::Shooter(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
{
	Init();
}

Shooter::~Shooter() 
{}

void Shooter::Init()
{
	self = std::unique_ptr<Self>(new Self);
	self->_background = Background::create(Core::resourceManager.Get<Render::Texture>("background01"));
	self->_gun = Gun::create(Core::resourceManager.Get<Render::Texture>("gun"), FPoint(Render::device.Width() * 0.5f, 80.f));

	// инициализация мишеней
	self->init_targets();
}

void Shooter::Reset() {
	self->_bullets.clear();
	self->_effects.clear();
	self->init_targets();
}

void Shooter::Draw()
{
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
	self->_acc += dt;

	if (self->_acc >= 10.f)
		self->_acc = self->SHOOT_DELAY;

	for (auto& target : self->_targets) {
		target->Update(dt);
	}

	// update всех выпущенных снарядов
	for (auto &bullet : self->_bullets)
		bullet->Update(dt);

	// update действующих эффектов
	for (auto &effect : self->_effects)
		effect->Update(dt);

	// проверка на столкновения пуль с мишенями
	self->collide();

	// удаляет отработавшие объекты из памяти
	self->remove();
}

bool Shooter::MouseDown(const IPoint &mouse_pos)
{
	if (self->_run) {
		// при возможности стреляем
		if (self->_bullets.size() < self->MAX_BULLETS && self->is_allow_to_shoot(mouse_pos)) {
			self->_bullets.push_back(Bullet::create(Core::resourceManager.Get<Render::Texture>("serious_bomb2"),
				self->BULLET_RESPAWN_POINT, Config::get("Speed")));
			self->_bullets.back()->InitDirection(mouse_pos);
			self->_acc = .0f;
		}
	}
	return false;
}

void Shooter::MouseMove(const IPoint &mouse_pos)
{
	if (self->_run) {
		// не поворачиваем пушку на себя
		if (mouse_pos.y <= self->BOTTOM_MARGIN) {
			return;
		}
		// получаем угол наклона пушки, взависимости от положения курсора
		float dx = Render::device.Width() * 0.5f - mouse_pos.x;
		float dy = mouse_pos.y;
		auto th = atan2(dx, dy);
		th *= 180 / math::PI;
		// поворачиваем пушку
		self->_gun->Rotate(th);
	}
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
	else if (data == "Win") {
		self->_run = false;
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