#pragma once

class Interface : public GUI::Widget
{
public:
	Interface(const std::string& name, rapidxml::xml_node<>* elem);
	virtual ~Interface();

	void Draw() override;
	void Update(float dt) override;

	void AcceptMessage(const Message& message) override;

	bool MouseDown(const IPoint& mouse_pos) override;
	void MouseMove(const IPoint& mouse_pos) override;
	void MouseUp(const IPoint& mouse_pos) override;

	void KeyPressed(int keyCode) override;
	void CharPressed(int unicodeChar) override;

private:
	void Init();

private:
	class Self;
	Self* self;
};
