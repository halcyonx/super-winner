#include "stdafx.h"
#include "ResultTable.h"

class ResultTable::Self {
public:
	FPoint _pos;
	int _scored;
	int _targets;
	bool _show;
};

std::unique_ptr<ResultTable> ResultTable::create(int scored, int targets, const FPoint& pos) {
	return std::make_unique<ResultTable>(scored, targets, pos);
}

ResultTable::ResultTable(int scored, int targets, const FPoint& pos) {
	self = new Self;
	self->_scored = scored;
	self->_targets = targets;
	self->_pos = pos;
	self->_show = false;
}

ResultTable::~ResultTable() {
	delete self;
}

void ResultTable::Draw() {
	if (self->_show) {
		Render::BindFont("kids");
		auto str = "Result: " + utils::lexical_cast(self->_scored) + "/" + utils::lexical_cast(self->_targets);
		Render::PrintString(self->_pos, str, 1.f, CenterAlign);
	}
}

void ResultTable::Show() {
	self->_show = true;
}

void ResultTable::Hide() {
	self->_show = false;
}

void ResultTable::SetScored(int scored, int targets) {
	self->_scored = scored;
	self->_targets = targets;
}