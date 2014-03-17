#include "ClassRoom.h"
#include "Human.h"
#include "HumanManager.h"
#include "Cell.h"
#include "boost\foreach.hpp"
#include <queue>
#include "Floor.h"

ClassRoom::ClassRoom(Floor* floor)
{
	ownerFloor = floor;
}


ClassRoom::~ClassRoom(void)
{
}

bool ClassRoom::ParseSchedule(boost::property_tree::ptree ptree)
{
	bool ret = true;
	using namespace boost::property_tree;
	using namespace std;
	try{
		int id = ptree.get<int>("<xmlattr>.classroom");
		BOOST_FOREACH(ptree::value_type& v, ptree)
		{
			if(v.first == "Monday"|| v.first == "Tuesday"||v.first == "Wednesday"||v.first == "Thursday"||v.first == "Friday")
			{
				std::map<std::string, bool> daymap;
				BOOST_FOREACH(ptree::value_type& vv, v.second)
				{
					if(vv.first == "First" || vv.first == "Second" || vv.first == "Third" || vv.first == "Forth")
					{
						if(vv.second.data() == "Y")
						{
							daymap.insert(std::make_pair<std::string, bool>(vv.first, true));
						} else {
							daymap.insert(std::make_pair<std::string, bool>(vv.first, false));
						}
					}
				}

				classSchedule.insert(std::make_pair<std::string, std::map<std::string, bool> >(v.first, daymap));
			}
		}
	}catch(std::exception e) {
		ret = false;
	}

	return ret;
}

bool ClassRoom::Parse(boost::property_tree::ptree ptree)
{
	bool ret = true;
	using namespace boost::property_tree;
	using namespace std;
	try{
		id = ptree.get<int>("<xmlattr>.id");
		BOOST_FOREACH(ptree::value_type& v, ptree)
		{
			if(v.first == "Position") {
				pos.x = v.second.get<int>("<xmlattr>.x");
				pos.y = v.second.get<int>("<xmlattr>.y");
			} else if(v.first == "Size") {
				size.x = v.second.get<int>("<xmlattr>.width");
				size.y = v.second.get<int>("<xmlattr>.length");
			} else if(v.first == "Orientation") {
				orientation.x = v.second.get<int>("<xmlattr>.x");
				orientation.y = v.second.get<int>("<xmlattr>.y");
			} else if(v.first == "Exits") {
				BOOST_FOREACH(ptree::value_type& vv, v.second)
				{
					if(vv.first == "Exit")
					{
						iPoint point;
						point.x = vv.second.get<int>("<xmlattr>.x");
						point.y = vv.second.get<int>("<xmlattr>.y");
						exits.push_back(point);
					}
				}
			} else {
			}
		}
	}catch(std::exception e) {
		ret = false;
	}

	return ret;
}

void ClassRoom::SetupCells(Cell** cells)
{
	int maxLen = 0;
	for(std::vector<iPoint>::iterator iter = exits.begin(); iter != exits.end(); iter++)
	{
		if(cells[iter->y][iter->x].GetShortestPathLength() > maxLen)maxLen = cells[iter->y][iter->x].GetShortestPathLength();
	}

	for(int i = this->pos.y; i < this->pos.y + this->size.y; i++)
	{
		for(int j = this->pos.x; j < this->pos.x + this->size.x; j++)
		{
			cells[i][j].SetShortestPathLength(LONG_MAX);
		}
	}

	std::queue<iPoint> cellque;
	for(std::vector<iPoint>::iterator iter = exits.begin(); iter != exits.end(); iter++)
	{
		cells[iter->y][iter->x].SetShortestPathLength(maxLen);
		cellque.push(*iter);
	}

	long length;
	while(!cellque.empty())
	{
		iPoint pos = cellque.front();
		cellque.pop();
		if(!(pos.x >= this->pos.x && pos.x < this->pos.x + this->size.x 
			&& pos.y >= this->pos.y && pos.y < this->size.y + this->pos.y))continue;
		length = cells[pos.y][pos.x].GetShortestPathLength();

		if(cells[pos.y - 1][pos.x - 1].GetShortestPathLength() > length + 1
			&&cells[pos.y - 1][pos.x - 1].GetType() == Cell::CT_FLOOR 
			&& (pos.x - 1 >= this->pos.x && pos.x-1 < this->pos.x + this->size.x 
			&& pos.y-1 >= this->pos.y && pos.y-1 < this->size.y + this->pos.y)){
				cellque.push(iPoint(pos.x - 1, pos.y - 1));
				cells[pos.y - 1][pos.x - 1].SetShortestPathLength(length + 1);
		}
		if(cells[pos.y - 1][pos.x].GetShortestPathLength() > length + 1
			&&cells[pos.y - 1][pos.x].GetType() == Cell::CT_FLOOR 
			&&(pos.x >= this->pos.x && pos.x < this->pos.x + this->size.x 
			&& pos.y-1 >= this->pos.y && pos.y-1 < this->size.y + this->pos.y)){
				cellque.push(iPoint(pos.x, pos.y - 1));
				cells[pos.y - 1][pos.x].SetShortestPathLength(length + 1);
		}
		if(cells[pos.y - 1][pos.x + 1].GetShortestPathLength() > length + 1
			&&cells[pos.y - 1][pos.x + 1].GetType() == Cell::CT_FLOOR 
			&&(pos.x+1 >= this->pos.x && pos.x+1 < this->pos.x + this->size.x 
			&& pos.y-1 >= this->pos.y && pos.y-1 < this->size.y + this->pos.y) ){
				cellque.push(iPoint(pos.x + 1, pos.y - 1));
				cells[pos.y - 1][pos.x + 1].SetShortestPathLength(length + 1);
		}

		if(cells[pos.y][pos.x - 1].GetShortestPathLength() > length + 1
			&&cells[pos.y][pos.x - 1].GetType() == Cell::CT_FLOOR 
			&&(pos.x-1 >= this->pos.x && pos.x-1 < this->pos.x + this->size.x 
			&& pos.y >= this->pos.y && pos.y < this->size.y + this->pos.y) ){
				cellque.push(iPoint(pos.x - 1, pos.y));
				cells[pos.y][pos.x - 1].SetShortestPathLength(length + 1);
		}
		if(cells[pos.y][pos.x + 1].GetShortestPathLength() > length + 1
			&&cells[pos.y][pos.x + 1].GetType() == Cell::CT_FLOOR 
			&&(pos.x+1 >= this->pos.x && pos.x+1 < this->pos.x + this->size.x 
			&& pos.y >= this->pos.y && pos.y < this->size.y + this->pos.y) ){
				cellque.push(iPoint(pos.x + 1, pos.y));
				cells[pos.y][pos.x + 1].SetShortestPathLength(length + 1);
		}

		if(cells[pos.y + 1][pos.x - 1].GetShortestPathLength() > length + 1
			&&cells[pos.y + 1][pos.x - 1].GetType() == Cell::CT_FLOOR 
			&&(pos.x-1 >= this->pos.x && pos.x-1 < this->pos.x + this->size.x 
			&& pos.y+1 >= this->pos.y && pos.y+1 < this->size.y + this->pos.y) ){
				cellque.push(iPoint(pos.x - 1, pos.y + 1));
				cells[pos.y + 1][pos.x - 1].SetShortestPathLength(length + 1);
		}
		if(cells[pos.y + 1][pos.x].GetShortestPathLength() > length + 1
			&&cells[pos.y + 1][pos.x].GetType() == Cell::CT_FLOOR 
			&&(pos.x >= this->pos.x && pos.x < this->pos.x + this->size.x 
			&& pos.y+1 >= this->pos.y && pos.y+1 < this->size.y + this->pos.y) ){
				cellque.push(iPoint(pos.x, pos.y + 1));
				cells[pos.y + 1][pos.x].SetShortestPathLength(length + 1);
		}
		if(cells[pos.y + 1][pos.x + 1].GetShortestPathLength() > length + 1
			&&cells[pos.y + 1][pos.x + 1].GetType() == Cell::CT_FLOOR 
			&&(pos.x+1 >= this->pos.x && pos.x+1 < this->pos.x + this->size.x 
			&& pos.y+1 >= this->pos.y && pos.y+1 < this->size.y + this->pos.y) ){
				cellque.push(iPoint(pos.x + 1, pos.y + 1));
				cells[pos.y + 1][pos.x + 1].SetShortestPathLength(length + 1);
		}
	}
}

int ClassRoom::GetCapacity()
{
	int ret = 0;
	for(int i = pos.y; i < pos.y + size.y; i++)
	{
		for(int j = pos.x; j < pos.x + size.x; j++)
		{
			if(cells[i][j].GetType() == Cell::CT_DC)
			{
				ret++;
			}
		}
	}

	return ret;
}

bool ClassRoom::DistributeHuman(std::vector<Human*>& humans, int scale)
{
	int capacity = GetCapacity();
	if(capacity / scale > 3)
	{
		for( int i = 0; i < scale; )
		{
			int rx = pos.x + rand() % size.x;
			int ry = pos.y + rand() % size.y;
			if(orientation.x == 1)
			{
				if(cells[ry][rx].GetType() == Cell::CT_DC && cells[ry][rx - 1].GetType() == Cell::CT_FLOOR && !cells[ry][rx - 1].HumanIn())
				{
					cells[ry][rx-1].SetHumanIn(true);
					Human *human = new Human();
					human->SetCells(cells);
					human->MoveTo(iPoint(rx-1, ry, ownerFloor->GetHeight()));
					//HumanManager::GetInstance().AddHuman(human);
					humans.push_back(human);
					i++;
				}
			} else if (orientation.x == -1) {
				if(cells[ry][rx].GetType() == Cell::CT_DC && cells[ry][rx+1].GetType() == Cell::CT_FLOOR && !cells[ry][rx+1].HumanIn())
				{
					cells[ry][rx+1].SetHumanIn(true);
					Human *human = new Human();
					human->SetCells(cells);
					human->MoveTo(iPoint(rx+1, ry, ownerFloor->GetHeight()));
					humans.push_back(human);
					i++;
					//this->humans.push_back(human);
					//HumanManager::GetInstance().AddHuman(human);
				}
			}

		}
	}
	else {
		int step = capacity / scale;
		if(step == 0)step = 1;
		for(int i = pos.y; i < pos.y + size.y; i+=step)
		{
			for(int j = pos.x; j < pos.x + size.x; j++)
			{
				if(orientation.x == 1)
				{
					if(cells[i][j].GetType() == Cell::CT_DC && cells[i][j - 1].GetType() == Cell::CT_FLOOR && !cells[i][j - 1].HumanIn())
					{
						cells[i][j-1].SetHumanIn(true);
						Human *human = new Human();
						human->SetCells(cells);
						human->MoveTo(iPoint(j-1, i, ownerFloor->GetHeight()));
						//HumanManager::GetInstance().AddHuman(human);
						humans.push_back(human);
					}
				} else if (orientation.x == -1) {
					if(cells[i][j].GetType() == Cell::CT_DC && cells[i][j+1].GetType() == Cell::CT_FLOOR && !cells[i][j+1].HumanIn())
					{
						cells[i][j+1].SetHumanIn(true);
						Human *human = new Human();
						human->SetCells(cells);
						human->MoveTo(iPoint(j+1, i, ownerFloor->GetHeight()));
						humans.push_back(human);
						//this->humans.push_back(human);
						//HumanManager::GetInstance().AddHuman(human);
					}

				}
			}
		}
	}
	return true;
}

int ClassRoom::GetId()
{
	return id;
}

void ClassRoom::SetCells(Cell** cls)
{
	cells = cls;
}

void ClassRoom::Evacuate()
{
}