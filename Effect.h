#pragma once

class Effect {
public:
	static std::unique_ptr<Effect> create(const FPoint& pos);

	Effect(const FPoint& pos);
	~Effect();

	void Draw();
	void Update(float dt);
	void Start();
	void Stop();
	bool IsActive() const;
	bool Expires() const;
	bool Alive(bool a = true);

private:
	class Self;
	Self *self;
};