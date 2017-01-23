#pragma once

class Target {
public:
	static std::unique_ptr<Target> create(Render::Texture* tex, const FPoint& pos, const FPoint& dir, float scale);

	Target(Render::Texture* tex, const FPoint& pos, const FPoint& dir, float scale);
	~Target();
	void Draw();
	void Update(float dt);
	void SetScale(float scale);
	void SetDirection(const FPoint& dir);
	const IRect& GetRect() const;
private:
	class Self;
	std::unique_ptr<Self> self;
};