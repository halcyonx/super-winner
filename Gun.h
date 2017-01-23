#pragma once

class Gun {
public:
	static std::unique_ptr<Gun> create(Render::Texture* tex, const FPoint& pos);

	Gun(Render::Texture* tex, const FPoint& pos);
	~Gun();
	void Draw();
	void Rotate(float angle);
private:
	class Self;
	Self* self;
};