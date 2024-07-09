#pragma once

class StatWindow {
public:
	StatWindow() {}

	void open();
	void draw();

	~StatWindow() {}
private:
	bool display = false;
};