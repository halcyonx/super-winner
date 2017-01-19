#pragma once

class Timer {
public:
	static std::unique_ptr<Timer> create(int t, const FPoint& pos = FPoint(100.f, 150.f), bool reverse = true);

	Timer(int t, const FPoint& pos, bool reverse);
	~Timer();

	void Start();
	void Stop();
	void Reset(int threshold);
	int Threshold() const;
	void Update(float dt);
	bool Expired() const;
	void Draw();
private:
	class Self;
	Self *self;
};