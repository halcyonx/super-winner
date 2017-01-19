#pragma once

class Target {
public:
	static std::shared_ptr<Target> create(Render::Texture* tex, const math::Vector3& pos, const math::Vector3& dir, float scale);

	Target(Render::Texture* tex, const math::Vector3& pos, const math::Vector3& dir, float scale);
	~Target();
	void Draw();
	void Update(float dt);
	void SetScale(float scale);
	void SetDirection(const math::Vector3& dir);
	const IRect& GetRect() const;
private:
	class Self;
	Self* self;
};