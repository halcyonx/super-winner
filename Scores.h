#pragma once

class Scores {
public:
	static std::unique_ptr<Scores> create(int initial, const FPoint& pos = FPoint(Render::device.Width() * 0.90f, 150.f));

	Scores(int initial, const FPoint& pos);
	~Scores();
	void Set(int score);
	int GetScore() const;
	void Draw();
private:
	class Self;
	std::unique_ptr<Self> self;
};