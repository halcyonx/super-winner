#pragma once

class Gun {
public:
	static std::shared_ptr<Gun> create(Render::Texture* tex, const math::Vector3& pos);

	Gun(Render::Texture* tex, const math::Vector3& pos);
	~Gun();
	void Draw();
	void Update(float dt);
	void Shoot(float x, float y);
	void Rotate(float angle);
private:
	class Self;
	Self* self;
};