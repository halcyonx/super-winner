#pragma once
#include "Effect.h"

class SimpleEffect {
public:
	static std::unique_ptr<SimpleEffect> create(const FPoint& pos);

	SimpleEffect(const FPoint& pos);
	~SimpleEffect();

	void Draw();
	void Update(float dt);
	void SetPos(const FPoint& pos);

private:
	class Self;
	Self *self;
};