#pragma once

class Cursor {
public:
	static std::unique_ptr<Cursor> create(Render::Texture* tex = Core::resourceManager.Get<Render::Texture>("cursor_red"));

	Cursor(Render::Texture* tex);
	~Cursor();
	void Draw();

private:
	class Self;
	Self *self;
};