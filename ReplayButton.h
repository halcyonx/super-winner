#pragma once

class ReplayButton {
public:
	static std::unique_ptr<ReplayButton> create(
		Render::Texture *normal = Core::resourceManager.Get<Render::Texture>("replay_normal"),
		Render::Texture *active = Core::resourceManager.Get<Render::Texture>("replay_active"),
		Render::Texture *pressed = Core::resourceManager.Get<Render::Texture>("replay_pressed"),
		const FPoint& pos = FPoint(Render::device.Width() * 0.5f, Render::device.Height() * 0.5f));

	ReplayButton(Render::Texture *normal, Render::Texture *active, Render::Texture *pressed, const FPoint& pos);
	~ReplayButton();
	void Draw();
	void Show();
	void Hide();
	bool MouseOn(const IPoint& mouse_pos);
	void Update(float dt);
	void MouseMove(const IPoint& mouse_pos);
	bool MouseDown(const IPoint& mouse_pos);
	bool MouseUp(const IPoint& mouse_pos);

private:
	class Self;
	std::unique_ptr<Self> self;
};