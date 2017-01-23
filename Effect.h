#pragma once

class Effect {
public:
	static std::unique_ptr<Effect> create(const std::string& name, const FPoint& pos, bool splined = true);

	Effect(const std::string& name, const FPoint& pos, bool splined);
	~Effect();

	void Draw();
	void Update(float dt);
	void Start();
	void Stop();
	void SetPos(const FPoint& pos);
	bool IsActive() const;
	bool Expires() const;
	bool Alive(bool a = true);

private:
	class Self;
	Self *self;
};