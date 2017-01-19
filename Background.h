#pragma once

class Background {
public:
	static std::shared_ptr<Background> create(Render::Texture* tex);
	
	Background(Render::Texture* tex);
	~Background();
	void Draw();
private:
	class Self;
	Self* self;
};