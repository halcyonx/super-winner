#pragma once

class Background {
public:
	static std::unique_ptr<Background> create(Render::Texture* tex);
	
	Background(Render::Texture* tex);
	~Background();
	void Draw();
private:
	class Self;
	std::unique_ptr<Self> self;
};