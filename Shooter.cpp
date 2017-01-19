#include "stdafx.h"
#include "Shooter.h"
#include "Background.h"
#include "Target.h"
#include "Gun.h"
#include "Bullet.h"
#include "Timer.h"
#include "GameUtils.h"
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
	{}

	// ���������� ����, ������� ����� ���� �������� ������������
	const unsigned int MAX_BULLETS = 3;
	const float SHOOT_DELAY = .5f;

	// _params ������ ��� �������� ���������� ���� (CountTarget, Speed, Time)
	std::map<std::string, int> _params;

	type(Background) _background;
	type(Gun) _gun;
	std::unique_ptr<Timer> _main_timer;
	std::list<type(Target)> _targets;
	std::list<type(Bullet)> _bullets;
	float _timer, _acc;

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
					auto temp = iter++;
					_targets.erase(temp);
					bullet->Stop();
					break;
				}
		}
	}
};

Shooter::Shooter(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
{
	Init();
}

Shooter::~Shooter() {
	delete self;
}

void Shooter::Init()
{
	self = new Self;

	// ������������� ���������� ���� �� input.txt
	auto stream = Core::fileSystem.OpenRead("input.txt");
	IO::TextReader reader(stream.get());
	std::string line;
	line = reader.ReadAsciiLine();
	while (line != "") {
		auto vec = game_utils::split(line, '=');
		self->_params[vec[0]] = std::stoi(vec[1]);
		line = reader.ReadAsciiLine();
	}

	self->_background = Background::create(Core::resourceManager.Get<Render::Texture>("background01"));
	self->_gun = Gun::create(Core::resourceManager.Get<Render::Texture>("gun"), math::Vector3(Render::device.Width() * 0.5f, 80, 0));
	self->_main_timer = Timer::create(self->_params["Time"]);
	self->_main_timer->Start();

	// ������������� �������
	for (int i = 0; i < self->_params["CountTarget"]; ++i) {
		self->_targets.push_back(
			Target::create(
				Core::resourceManager.Get<Render::Texture>("blue_bubble"),
				game_utils::random_vec(300, 600), // ��������� ��������� �������
				game_utils::random_vec(-5, 5), // ��������� �����������
				game_utils::random_float() // ��������� ������
			)
		);
	}
}

void Shooter::Draw()
{
	// draw stuff
	self->_background->Draw();
	self->_main_timer->Draw();

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
	self->_timer += dt;
	self->_acc += dt;

	// ���� ����� ���� �������
	if (self->_main_timer->Expired()) {

	}
	else
		self->_main_timer->Update(dt);

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
	if (Core::mainInput.GetMouseRightButton())
	{
	}
	else 
	{
		if (self->_bullets.size() < self->MAX_BULLETS && self->is_allow_to_shoot(mouse_pos)) {
			self->_bullets.push_back(Bullet::create(Core::resourceManager.Get<Render::Texture>("cannonball"),
				FPoint(Render::device.Width() * 0.5f, -30), self->_params["Speed"]));
			self->_bullets.back()->SetStartPoint(mouse_pos);
			self->_bullets.back()->FlyTo(FPoint(mouse_pos.x, mouse_pos.y));
			self->_acc = .0f;
		}
	}
	return false;
}

void Shooter::MouseMove(const IPoint &mouse_pos)
{
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
