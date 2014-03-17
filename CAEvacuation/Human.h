#pragma once
#include "commantypes.h"
#include <list>
#include <map>
#include <fstream>

class Cell;
class Human
{
public:
	Human(void);
	~Human(void);
	void SetPosition(iPoint pos);
	iPoint GetPosition();
	long GetId();
	void LogPath(std::ofstream& ofs);
	void MoveTo(iPoint pos);
	void SetCells(Cell** cells);

	void TakeOneStep();
	bool IsOut(){return out;};
	void SetOut(bool var);
private:
	long id;
	Cell ** cells;
	bool out;
	iPoint mCurrentPos;
	std::list<iPoint> path;

	static long HumanNum;
};

