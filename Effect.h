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
	//class Self;
	//std::unique_ptr<Self> self;
	EffectsContainer _effCont;
	ParticleEffectPtr _eff;
	TimedSpline<FPoint> _spline;
	std::vector<FPoint> _coords;
	FPoint _pos;
	float _dt;
	float _t;
	bool _active;
	bool _alive;
	bool _splined;
	Effect *self;
};