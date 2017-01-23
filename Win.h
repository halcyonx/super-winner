#pragma once

class WinPlate {
public:
	static std::unique_ptr<WinPlate> create(
		Render::Texture* tex = Core::resourceManager.Get<Render::Texture>("win"), 
		const FPoint& pos = FPoint(Render::device.Width() * 0.5f, Render::device.Height() * 0.5f));

	WinPlate(Render::Texture* tex, const FPoint& pos);
	~WinPlate();

	void Show();
	void Hide();
	void Draw();

private:
	class Self;
	std::unique_ptr<Self> self;
};