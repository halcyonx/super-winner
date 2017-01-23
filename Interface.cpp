#include "stdafx.h"
#include "Interface.h"
#include "Timer.h"
#include "ReplayButton.h"
#include "Cursor.h"
#include "Config.h"
#include "Scores.h"
#include "Win.h"
#include "ResultTable.h"

#define ptr(t) \
std::unique_ptr<t>

class Interface::Self {
public:
	ptr(Timer) _main_timer;
	ptr(ReplayButton) _replay_button;
	ptr(Cursor) _cursor;
	ptr(Scores) _scores;
	ptr(WinPlate) _winplate;
	ptr(ResultTable) _results;
};

Interface::Interface(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
{
	Init();
}

Interface::~Interface()
{}

void Interface::Init() {
	self = std::unique_ptr<Self>(new Self);
	self->_main_timer = Timer::create(Config::get("Time"));
	self->_replay_button = ReplayButton::create();
	self->_cursor = Cursor::create();
	self->_winplate = WinPlate::create();
	self->_scores = Scores::create(0);
	self->_results = ResultTable::create(0, Config::get("CountTarget"));
	self->_main_timer->Start();
	ShowCursor(false);
	long Style = GetWindowLong(Core::appInstance->GetMainWindow()->GetHandle(), GWL_STYLE);
	Style &= ~WS_MAXIMIZEBOX;
	SetWindowLong(Core::appInstance->GetMainWindow()->GetHandle(), GWL_STYLE, Style);
	Core::mainInput.SetMousePos(Render::device.Width() * 0.5f, Render::device.Height() * 0.5f);
}

void Interface::Draw() {
	self->_cursor->Draw();
	self->_main_timer->Draw();
	self->_replay_button->Draw();
	self->_scores->Draw();
	self->_winplate->Draw();
	self->_results->Draw();
}

void Interface::Update(float dt) {
	// если время игры истекло
	if (self->_main_timer->IsActive() && self->_main_timer->Expired()) {
		Core::guiManager.getLayer("TestLayer")->getWidget("Shooter")->AcceptMessage(Message("StopGame", "StopGame"));
		self->_replay_button->Show();
		self->_results->SetScored(self->_scores->GetScore(), Config::get("CountTarget"));
		self->_results->Show();
	}
	else {
		if (self->_scores->GetScore() == Config::get("CountTarget") && self->_main_timer->IsActive()) {
			Core::guiManager.getLayer("TestLayer")->getWidget("Shooter")->AcceptMessage(Message("", "Win"));
			self->_winplate->Show();
			self->_replay_button->Show();
			self->_main_timer->Stop();
			self->_results->SetScored(self->_scores->GetScore(), Config::get("CountTarget"));
			self->_results->Show();
		}
		self->_main_timer->Update(dt);
	}
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
		self->_results->Hide();
		self->_winplate->Hide();
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