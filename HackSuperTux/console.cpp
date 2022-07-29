#include "pch.h"
#include "console.h"
#include <string>

void Console::startConsole() {
	AllocConsole();
	freopen_s(&f, "CONOUT$", "w", stdout);

	flyingHacks = false;
	speed2x = false;
	jump2x = false;
	immortalityHacks = false;
	spectateModeOn = false;
}

void Console::endConsole() {
	fclose(f);
	FreeConsole();
}
void Console::toggleFlyingHacks() {
	flyingHacks = !flyingHacks;
	updateConsole();
}
void Console::toggleSpeed2xHacks() {
	speed2x = !speed2x;
	updateConsole();
}
void Console::toggleJump2xHacks() {
	jump2x = !jump2x;
	updateConsole();
}
void Console::toggleImmortalityHacks() {
	immortalityHacks = !immortalityHacks;
	updateConsole();
}
void Console::toggleSpectateMode() {
	spectateModeOn = !spectateModeOn;
	updateConsole();
}

void Console::updateConsole() {
	system("CLS"); //clear the console
	std::string immortalityHacksStr = immortalityHacks ? "X" : " ";
	std::string spectateModeStr = spectateModeOn ? "X" : " ";
	std::string flyingHacksStr = flyingHacks ? "X" : " ";
	std::string speed2xStr = speed2x ? "X" : " ";
	std::string jump2xStr = jump2x ? "X" : " ";
	std::cout << "X------------------X" << std::endl;
	std::cout << "|   SUPERTUX HACK  |" << std::endl;
	std::cout << "|------------------|" << std::endl;
	std::cout << "|Immortality - <" << immortalityHacksStr << ">-|" << std::endl;
	std::cout << "|Spectate    - <" << spectateModeStr << ">-|" << std::endl;
	std::cout << "|Flying Hack - <" << flyingHacksStr << ">-|" << std::endl;
	std::cout << "|Speed Hack  - <" << speed2xStr << ">-|" << std::endl;
	std::cout << "|Jump2x Hack - <" << jump2xStr << ">-|" << std::endl;
	std::cout << "| F10 - Quit       |" << std::endl;
	std::cout << "X------------------X" << std::endl;
}