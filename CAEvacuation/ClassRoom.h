#pragma once
#include <string>
#include <vector>
#include "boost\property_tree\ptree.hpp"
#include "commantypes.h"
#include <map>

class Cell;
class Human;
class Floor;
class ClassRoom
{
public:
	bool Parse(boost::property_tree::ptree pt);
	void SetupCells(Cell** cells);
	bool ParseSchedule(boost::property_tree::ptree pt);
	ClassRoom(Floor * parent);
	~ClassRoom(void);
	int GetId();

	void Evacuate();

	bool DistributeHuman(std::vector<Human*>& humans, int scal = 1);
	void SetCells(Cell**);

private:
	int GetCapacity();

private:
	int id;
	iPoint pos;
	iPoint size;
	iPoint orientation;
	std::vector<iPoint> exits; 
	Floor* ownerFloor;


	Cell** cells;
	std::vector<bool> schedule;
	std::map<std::string, std::map<std::string, bool> > classSchedule;
};

