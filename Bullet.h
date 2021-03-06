#pragma once
#include "Effect.h"

class Bullet {
public:
	static std::unique_ptr<Bullet> create(Render::Texture* tex, const FPoint& pos, int speed);

	Bullet(Render::Texture* tex, const FPoint& pos, int speed);
	~Bullet();
	void Draw();
	void Update(float dt);
	void InitDirection(const IPoint& mouse_pos);
	bool IsFly() const;
	void Stop() const;
	const IRect& GetRect() const;
	const FPoint& GetPos() const;
	std::unique_ptr<Effect> effect();
private:
	class Self;
	std::unique_ptr<Self> self;
};