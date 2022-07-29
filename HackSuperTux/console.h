#pragma once

class Console {
public:
	void startConsole();
	void endConsole();
	void toggleFlyingHacks();
	void toggleSpeed2xHacks();
	void toggleJump2xHacks();
	void toggleImmortalityHacks();
	void toggleSpectateMode();
private:
	FILE* f;
	bool flyingHacks;
	bool speed2x;
	bool jump2x;
	bool immortalityHacks;
	bool spectateModeOn;

	void updateConsole();
};