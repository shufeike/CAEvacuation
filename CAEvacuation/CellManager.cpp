#include "CellManager.h"

template<> CellManager* Singleton<CellManager>::ms_Singleton = 0;

CellManager::CellManager(void)
{
}


CellManager::~CellManager(void)
{
}


void CellManager::AddCells(Cell** cells)
{
	floors.push_back(cells);
}

Cell** CellManager::GetCells(int floor)
{
	if(floor >= 0 && floor < floors.size())
	{
		return floors[floor];
	}
	else 
	{
		return 0;
	}
}