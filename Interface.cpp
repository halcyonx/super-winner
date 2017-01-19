#include "stdafx.h"
#include "Interface.h"
#include "Timer.h"

class Interface::Self {
public:
	struct ReplayButton;
	std::unique_ptr<Timer> _main_timer;
	std::unique_ptr<ReplayButton> _replay_button;

	struct ReplayButton {
		Render::Texture* _tex;
		Render::Texture* _replay_normal;
		Render::Texture* _replay_active;
		Render::Texture* _replay_pressed;
		bool _visible;
		FPoint _pos;
		IRect _rect;

		static std::unique_ptr<ReplayButton> create(
			Render::Texture *normal = Core::resourceManager.Get<Render::Texture>("replay_normal"),
			Render::Texture *active = Core::resourceManager.Get<Render::Texture>("replay_active"),
			Render::Texture *pressed = Core::resourceManager.Get<Render::Texture>("replay_pressed"), 
			const FPoint& pos = FPoint(Render::device.Width() * 0.5f, Render::device.Height() * 0.5f)) {
			return std::make_unique<ReplayButton>(normal, active, pressed, pos);
		}

		ReplayButton(Render::Texture *normal, Render::Texture *active, Render::Texture *pressed, const FPoint& pos)
			: _tex(normal)
			, _replay_normal(normal)
			, _replay_active(active)
			, _replay_pressed(pressed)
			, _pos(pos)
			, _visible(false)
		{}

		void Draw() {
			if (_visible) {
				Render::device.PushMatrix();
				Render::device.MatrixTranslate(_pos);
				Render::device.MatrixTranslate(-_tex->_rect_width * 0.5f, -_tex->_rect_height * 0.5f, 0);
				_tex->Draw();
				Render::device.PopMatrix();
			}
		}

		void Show() {
			_visible = true;
		}

		void Hide() {
			_visible = false;
		}

		bool MouseOn(const IPoint& mouse_pos) {
			if (!_visible) return false;
			auto rect = _tex->getBitmapRect();
			_rect = IRect(_pos.x - rect.width * 0.5f, _pos.y - rect.height * 0.5f, rect.width, rect.height);
			if (_rect.Contains(mouse_pos))
				return true;
			return false;
		}

		void Update(float dt) {
			
		}

		void MouseMove(const IPoint& mouse_pos) {
			if (MouseOn(mouse_pos))
				_tex = _replay_active;
			else
				_tex = _replay_normal;
		}

		bool MouseDown(const IPoint& mouse_pos) {
			if (MouseOn(mouse_pos)) {
				_tex = _replay_pressed;
				return true;
			}
			return false;
		}

		bool MouseUp(const IPoint& mouse_pos) {
			if (MouseOn(mouse_pos)) {
				_tex = _replay_normal;
				return true;
			}
			return false;
		}
	};
};

Interface::Interface(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
{
	self = new Self;
	Init();
}

Interface::~Interface()
{
	delete self;
}

void Interface::Init() {
	self->_main_timer = Timer::create(5);
	self->_replay_button = Self::ReplayButton::create();
	self->_main_timer->Start();
}

void Interface::Draw() {
	self->_main_timer->Draw();
	self->_replay_button->Draw();
}

void Interface::Update(float dt) {
	// если время игры истекло
	if (self->_main_timer->IsActive() && self->_main_timer->Expired()) {
		Core::guiManager.getLayer("TestLayer")->getWidget("Shooter")->AcceptMessage(Message("StopGame", "StopGame"));
		Log::Debug("interface->stop");
		self->_replay_button->Show();
	}
	else
		self->_main_timer->Update(dt);
}

void Interface::AcceptMessage(const Message& message) {
	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
}

bool Interface::MouseDown(const IPoint& mouse_pos) {
	self->_replay_button->MouseDown(mouse_pos);
	return false;
}

void Interface::MouseMove(const IPoint& mouse_pos) {
	self->_replay_button->MouseMove(mouse_pos);
}

void Interface::MouseUp(const IPoint& mouse_pos) {
	if (self->_replay_button->MouseUp(mouse_pos) ) {
		self->_replay_button->Hide();
		self->_main_timer->Reset();
		self->_main_timer->Start();
		Core::guiManager.getLayer("TestLayer")->getWidget("Shooter")->AcceptMessage(Message("RestartGame", "RestartGame"));
		Log::Debug("interface->restart");
	}
}

void Interface::KeyPressed(int keyCode) {

}

void Interface::CharPressed(int unicodeChar) {

}