#pragma once
#include <string>

class Utilities
{
public:
	static void AskToExit();
	static void AskToExit(std::string info);
	static void PressKeyToExit(std::string info, int error = 0);

private:
	Utilities(void);
	~Utilities(void);
};

