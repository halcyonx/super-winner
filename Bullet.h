#pragma once

class Bullet {
public:
	static std::shared_ptr<Bullet> create(Render::Texture* tex, const FPoint& pos, int speed);

	Bullet(Render::Texture* tex, const FPoint& pos, int speed);
	~Bullet();
	void Draw();
	void Update(float dt);
	void FlyTo(const FPoint& p);
	void SetStartPoint(const IPoint& mouse_pos);
	bool IsFly() const;
	void Stop() const;
	const IRect& GetRect() const;
private:
	class Self;
	Self* self;
};