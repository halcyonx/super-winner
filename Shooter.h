#pragma once

class Shooter : public GUI::Widget
{
public:
	Shooter(const std::string& name, rapidxml::xml_node<>* elem);
	virtual ~Shooter();

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
	void Reset();
private:
	class Self;
	Self* self;
};
