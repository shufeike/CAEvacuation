#include "HumanManager.h"
#include "Human.h"
#include <fstream>
#include "CellManager.h"

template<> HumanManager* Singleton<HumanManager>::ms_Singleton = 0;

HumanManager::HumanManager(void)
{
}


HumanManager::~HumanManager(void)
{
}

void HumanManager::Evacuate()
{
	while(!humans.empty()){
		std::vector<Human*>::iterator iter = humans.begin();
		while(iter != humans.end())
		{
			(*iter)->TakeOneStep();
			if((*iter)->IsOut())
			{
				outHumans.push_back(*iter);
				iter = humans.erase(iter);
			} else {
				iter++;
			}
		}
	}
}

bool HumanManager::AddHuman(Human* human)
{
	humans.push_back(human);
	return true;
}


void HumanManager::LogHumanPath()
{
	std::ofstream humanPaths("humanpaths.data");
	//for(std::map<long, Human*>::iterator iter = humans.begin(); iter != humans.end(); iter++)
	//{}
	for(std::vector<Human*>::iterator iter = outHumans.begin(); iter != outHumans.end(); iter++)
	{
		(*iter)->LogPath(humanPaths);
	}
}