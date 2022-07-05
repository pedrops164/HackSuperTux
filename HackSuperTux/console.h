#pragma once

class Console {
public:
	void startConsole();
	void endConsole();
	void toggleFlyingHacks();
	void toggleSpeed2xHacks();
	void toggleJump2xHacks();
private:
	FILE* f;
	bool flyingHacks;
	bool speed2x;
	bool jump2x;

	void updateConsole();
};