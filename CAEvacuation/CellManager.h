#pragma once
#include "Singleton.h"
#include <vector>
class Cell;
class CellManager : public Singleton<CellManager>
{
public:
	CellManager(void);
	~CellManager(void);
	void AddCells(Cell** cells);
	Cell** GetCells(int floor);
private:
	CellManager(const CellManager&);
	CellManager& operator= (const CellManager&);
	
	std::vector<Cell**> floors;

};

