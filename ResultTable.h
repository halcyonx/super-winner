#pragma once


class ResultTable {
public:
	static std::unique_ptr<ResultTable> create(
		int scored, 
		int targets, 
		const FPoint& pos = FPoint(Render::device.Width() * 0.5f, Render::device.Height() * 0.5f - 100.f));

	ResultTable(int scored, int targets, const FPoint& pos);
	~ResultTable();
	void Draw();
	void Show();
	void Hide();
	void SetScored(int scored, int targets);

private:
	class Self;
	std::unique_ptr<Self> self;
};