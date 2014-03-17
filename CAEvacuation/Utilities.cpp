#include "Utilities.h"
#include <iostream>

Utilities::Utilities(void)
{
}


Utilities::~Utilities(void)
{
}

void Utilities::AskToExit()
{
}

void Utilities::AskToExit(std::string info)
{
}

void Utilities::PressKeyToExit(std::string info, int error)
{
	std::cout << info << std::endl;
	std::cout << "Press any key to exit." << std::endl;
	getchar();
	exit(error);
}