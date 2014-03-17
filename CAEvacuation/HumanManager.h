#pragma once
#include <map>
#include <vector>
#include "Singleton.h"

class Human;
class HumanManager : public Singleton<HumanManager>
{
public:
	HumanManager(void);
	~HumanManager(void);
	bool AddHuman(Human* human);
	void LogHumanPath();
	void Evacuate();

private:
	std::vector<Human*> humans;

	std::vector<Human*> outHumans;

	HumanManager(const HumanManager&);
	HumanManager& operator= (const HumanManager&);
};

