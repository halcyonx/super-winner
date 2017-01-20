#include "stdafx.h"
#include "Interface.h"
#include "Timer.h"
#include "ReplayButton.h"
#include "Cursor.h"
#include "Config.h"
#include "Scores.h"

class Interface::Self {
public:
	std::unique_ptr<Timer> _main_timer;
	std::unique_ptr<ReplayButton> _replay_button;
	std::unique_ptr<Cursor> _cursor;
	std::unique_ptr<Scores> _scores;
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
	self->_main_timer = Timer::create(Config::get("Time"));
	self->_replay_button = ReplayButton::create();
	self->_cursor = Cursor::create();
	self->_scores = Scores::create(0);
	self->_main_timer->Start();
	ShowCursor(false);
	Core::mainInput.SetMousePos(Render::device.Width() * 0.5f, Render::device.Height() * 0.5f);
}

void Interface::Draw() {
	self->_cursor->Draw();
	self->_main_timer->Draw();
	self->_replay_button->Draw();
	self->_scores->Draw();
}

void Interface::Update(float dt) {
	// если время игры истекло
	if (self->_main_timer->IsActive() && self->_main_timer->Expired()) {
		Core::guiManager.getLayer("TestLayer")->getWidget("Shooter")->AcceptMessage(Message("StopGame", "StopGame"));
		self->_replay_button->Show();
	}
	else
		self->_main_timer->Update(dt);
}

void Interface::AcceptMessage(const Message& message) {
	const std::string& publisher = message.getPublisher();
	const std::string& data = message.getData();
	if (data == "ScoreAdd") {
		self->_scores->Set(self->_scores->GetScore() + 1);
	}
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
		self->_scores->Set(0);
	}
}

void Interface::KeyPressed(int keyCode) {

}

void Interface::CharPressed(int unicodeChar) {

}