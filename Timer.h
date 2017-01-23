#pragma once

class Timer {
public:
	static std::unique_ptr<Timer> create(int t, const FPoint& pos = FPoint(100.f, 150.f), bool reverse = true);

	Timer(int t, const FPoint& pos, bool reverse);
	~Timer();

	void Start();
	void Stop();
	void Reset();
	void Update(float dt);
	bool Expired() const;
	bool IsActive() const;
	void Draw();
private:
	class Self;
	std::unique_ptr<Self> self;
};