#include <fstream>
#include "Floor.h"
#include "Cell.h"
#include "ClassRoom.h"
#include "Utilities.h"
#include "boost\property_tree\ptree.hpp"
#include "boost\property_tree\xml_parser.hpp"
#include "boost\foreach.hpp"
#include "CellManager.h"
#include "Human.h"
#include <queue>

Floor::Floor()
	:mCells(0), mWidth(0), mLength(0), mHeight(0), mBaseFloor(0)
{}

Floor::Floor(std::string cellsFile)
	:mCells(0), mWidth(0), mLength(0), mHeight(0), mBaseFloor(0)
{
	LoadCells(cellsFile);
}

void Floor::LoadCells(std::string file)
{
	try
	{
		std::ifstream terrain(file);
		if(!terrain)
		{
			Utilities::PressKeyToExit("open file: " + file +"failed", -1);
		}

		float width, length;
		long id;
		float pw, pl, ph;
		int type;
		terrain>>width>>length;
		mCells = new Cell*[(int) length];
		for(int i = 0; i < length; i++)
		{
			mCells[i] = new Cell[(int)width];
		}

		while(!terrain.eof())
		{
			terrain >> id >> pw >> pl >> ph >> type;
			mCells[(int)pl][(int)pw].init(id, pw, pl, ph + mHeight, (Cell::CellType)type);
			if(type == Cell::CT_EXIT)
			{
				exits.push_back(iPoint(pw, pl, mHeight));
			}
		}

		terrain.close();
		mWidth = width;
		mLength = length;

	} catch(std::exception e) {
		Utilities::PressKeyToExit("Read file: " + file + " error." + e.what(), -1);
	}

	//CellManager::GetInstance().AddCells(mCells);
}

void Floor::LoadClassRooms(std::string classroomsFile)
{
	using namespace boost::property_tree;
	using namespace std;
	ptree pt;
	read_xml(classroomsFile, pt);
	BOOST_FOREACH(ptree::value_type &v1, pt.get_child("ClassRooms"))
	{  
        if(v1.first == "ClassRoom"){  
			ClassRoom* cr = new ClassRoom(this);
			cr->SetCells(mCells);
			cr->Parse(v1.second);
			//cr->SetupCells(mCells);
			mClassrooms.insert(std::make_pair<int, ClassRoom*>(cr->GetId(), cr));
        }  
	}
}

void Floor::LoadClassSchedules(std::string schedulesFile)
{
	using namespace boost::property_tree;
	using namespace std;
	ptree pt;
	read_xml(schedulesFile, pt);
	BOOST_FOREACH(ptree::value_type &v1, pt.get_child("Schedules"))
	{  
        if(v1.first == "Schedule"){  
			int id = v1.second.get<int>("<xmlattr>.classroom");
			ClassRoom* cr =mClassrooms.at(id);
			if(cr)cr->ParseSchedule(v1.second);
        }  
	}
}

void Floor::DistributeHumans()
{
	for(std::map<int, ClassRoom*>::iterator iter = mClassrooms.begin(); iter != mClassrooms.end(); iter++)
	{
		iter->second->DistributeHuman(mHumans, 200);
	}
}


Floor::~Floor(void)
{
	for(int i = 0; i < mLength; i++)
	{
		delete[] mCells[i];
	}
	
	delete[] mCells;
}

void Floor::SetupCells()
{
	Cell** cells = mCells;
	std::queue<iPoint> cellque;
	for(std::vector<iPoint>::iterator iter = exits.begin(); iter != exits.end(); iter++)
	{
		cells[iter->y][iter->x].SetShortestPathLength(0);
		cellque.push(*iter);
	}

	long length;
	while(!cellque.empty())
	{
		iPoint pos = cellque.front();
		cellque.pop();/*
		if(!(pos.x >= this->pos.x && pos.x < this->pos.x + this->size.x 
			&& pos.y >= this->pos.y && pos.y < this->size.y + this->pos.y))continue;*/
		length = cells[pos.y][pos.x].GetShortestPathLength();

		if(cells[pos.y - 1][pos.x - 1].GetShortestPathLength() > length + 1
			&&(cells[pos.y - 1][pos.x - 1].GetType() == Cell::CT_FLOOR 
				|| cells[pos.y - 1][pos.x - 1].GetType() == Cell::CT_INTERSECTION
				||cells[pos.y - 1][pos.x - 1].GetType() == Cell::CT_STAIR ) ){
				cellque.push(iPoint(pos.x - 1, pos.y - 1));
				cells[pos.y - 1][pos.x - 1].SetShortestPathLength(length + 1);
		}
		if(cells[pos.y - 1][pos.x].GetShortestPathLength() > length + 1
			&&(cells[pos.y - 1][pos.x].GetType() == Cell::CT_FLOOR 
				|| cells[pos.y - 1][pos.x ].GetType() == Cell::CT_INTERSECTION
				|| cells[pos.y - 1][pos.x].GetType() == Cell::CT_STAIR )){
				cellque.push(iPoint(pos.x, pos.y - 1));
				cells[pos.y - 1][pos.x].SetShortestPathLength(length + 1);
		}
		if(cells[pos.y - 1][pos.x + 1].GetShortestPathLength() > length + 1
			&&(cells[pos.y - 1][pos.x + 1].GetType() == Cell::CT_FLOOR 
				|| cells[pos.y - 1][pos.x + 1].GetType() == Cell::CT_INTERSECTION
				|| cells[pos.y - 1][pos.x + 1].GetType() == Cell::CT_STAIR) ){
				cellque.push(iPoint(pos.x + 1, pos.y - 1));
				cells[pos.y - 1][pos.x + 1].SetShortestPathLength(length + 1);
		}

		if(cells[pos.y][pos.x - 1].GetShortestPathLength() > length + 1
			&&(cells[pos.y][pos.x - 1].GetType() == Cell::CT_FLOOR 
				|| cells[pos.y][pos.x - 1].GetType() == Cell::CT_INTERSECTION
				|| cells[pos.y][pos.x - 1].GetType() == Cell::CT_STAIR) ){
				cellque.push(iPoint(pos.x - 1, pos.y));
				cells[pos.y][pos.x - 1].SetShortestPathLength(length + 1);
		}
		if(cells[pos.y][pos.x + 1].GetShortestPathLength() > length + 1
			&&(cells[pos.y][pos.x + 1].GetType() == Cell::CT_FLOOR 
				|| cells[pos.y][pos.x + 1].GetType() == Cell::CT_INTERSECTION
				|| cells[pos.y][pos.x + 1].GetType() == Cell::CT_STAIR) ){
				cellque.push(iPoint(pos.x + 1, pos.y));
				cells[pos.y][pos.x + 1].SetShortestPathLength(length + 1);
		}

		if(cells[pos.y + 1][pos.x - 1].GetShortestPathLength() > length + 1
			&&(cells[pos.y + 1][pos.x - 1].GetType() == Cell::CT_FLOOR 
				|| cells[pos.y + 1][pos.x - 1].GetType() == Cell::CT_INTERSECTION
				|| cells[pos.y + 1][pos.x - 1].GetType() == Cell::CT_STAIR )){
				cellque.push(iPoint(pos.x - 1, pos.y + 1));
				cells[pos.y + 1][pos.x - 1].SetShortestPathLength(length + 1);
		}
		if(cells[pos.y + 1][pos.x].GetShortestPathLength() > length + 1
			&&(cells[pos.y + 1][pos.x].GetType() == Cell::CT_FLOOR 
				|| cells[pos.y + 1][pos.x].GetType() == Cell::CT_INTERSECTION
				|| cells[pos.y + 1][pos.x].GetType() == Cell::CT_STAIR )){
				cellque.push(iPoint(pos.x, pos.y + 1));
				cells[pos.y + 1][pos.x].SetShortestPathLength(length + 1);
		}
		if(cells[pos.y + 1][pos.x + 1].GetShortestPathLength() > length + 1
			&&(cells[pos.y + 1][pos.x + 1].GetType() == Cell::CT_FLOOR 
				|| cells[pos.y + 1][pos.x + 1].GetType() == Cell::CT_INTERSECTION
				|| cells[pos.y + 1][pos.x + 1].GetType() == Cell::CT_STAIR )){
				cellque.push(iPoint(pos.x + 1, pos.y + 1));
				cells[pos.y + 1][pos.x + 1].SetShortestPathLength(length + 1);
		}
	}

	for(std::map<int, ClassRoom*>::iterator it = mClassrooms.begin(); it != mClassrooms.end(); it++)
	{
		it->second->SetupCells(mCells);
	}
}


void Floor::SetBaseFloor(Floor* floor)
{
	mBaseFloor = floor;
}

void Floor::SetHeight(int height)
{
	mHeight = height;
}

int Floor::GetHeight()
{
	return mHeight;
}

//待改进
bool Floor::HumanComeDown(Human* human)
{
	iPoint pos = human->GetPosition();
	Cell cell = mCells[pos.y][pos.x];
	int r = 0;
	while(!(cell.GetType() == Cell::CT_INTERSECTION && !cell.HumanIn() ))
	{
		r++;
		if(r > 8) break;
		for( int i = pos.y - r; i <= pos.y + r; i++)
		{
			for(int j = pos.x - r; j <= pos.x + r; j++)
			{
				cell = mCells[i][j];
				if(cell.GetType() == Cell::CT_INTERSECTION && !cell.HumanIn())
				{
					pos = iPoint(j, i);
					goto found;
				}
			}
		}
	}

	return false;

found:
	human->SetCells(mCells);
	mHumans.push_back(human);
	pos.z = mHeight;

	//未加入路径
	human->SetPosition(pos);
	human->SetOut(false);
	mCells[pos.y][pos.x].SetHumanIn(true);
	return true;
}

bool Floor::IsClear()
{
	bool ret = false;
	ret = mHumans.empty();
	if(ret && mBaseFloor)
	{
		ret = mOutHumans.empty();
	}
	return ret;
}

bool Floor::IntersectionAvailabel(const iPoint& pos)
{
	Cell cell = mCells[pos.y][pos.x];
	int r = 0;
	while(!(cell.GetType() == Cell::CT_INTERSECTION && !cell.HumanIn() ))
	{
		r++;
		if(r > 8) break;
		for( int i = pos.y - r; i <= pos.y + r; i++)
		{
			for(int j = pos.x - r; j <= pos.x + r; j++)
			{
				cell = mCells[i][j];
				if(cell.GetType() == Cell::CT_INTERSECTION && !cell.HumanIn())
				{
					goto found;
				}
			}
		}
	}

	return false;

found:
	return true;
}

void Floor::EvacuateOneStep()
{
	std::vector<Human*>::iterator iter = mHumans.begin();
	while(iter != mHumans.end())
	{
		Human* human = *iter;

		if(human->IsOut()){
			if(!mBaseFloor){
				mOutHumans.push_back(human);
				mCells[human->GetPosition().y][human->GetPosition().x].SetHumanIn(false);
				iter = mHumans.erase(iter);
			}
			else if(mBaseFloor->IntersectionAvailabel(human->GetPosition()))
			{
				mCells[human->GetPosition().y][human->GetPosition().x].SetHumanIn(false);
				mBaseFloor->HumanComeDown(human);
				iter = mHumans.erase(iter);
			}
			else
			{
				human->MoveTo(human->GetPosition());
				iter++;
			}

			continue;
		}
		else
		{
			iPoint pos = human->GetPosition();
			long len[9];
			len[0] = mCells[pos.y][pos.x].GetShortestPathLength();
			len[1] = mCells[pos.y - 1][pos.x - 1].GetShortestPathLength();
			len[2] = mCells[pos.y - 1][pos.x].GetShortestPathLength();
			len[3] = mCells[pos.y - 1][pos.x + 1].GetShortestPathLength();
			len[4] = mCells[pos.y][pos.x - 1].GetShortestPathLength();
			len[5] = mCells[pos.y][pos.x + 1].GetShortestPathLength();
			len[6] = mCells[pos.y + 1][pos.x - 1].GetShortestPathLength();
			len[7] = mCells[pos.y + 1][pos.x].GetShortestPathLength();
			len[8] = mCells[pos.y + 1][pos.x + 1].GetShortestPathLength();
			int min = len[0];
			for(int i = 0; i < 9; i++)
			{
				if(min > len[i]) 
				{
					min = len[i];
				}
			}

			std::vector<int> minPos;
			for(int i = 0; i < 9; i++)
			{
				if(min == len[i])
				{
					minPos.push_back(i);
				}
			}
	
			int i = rand() %  minPos.size();
			int curse = minPos[i];
			iPoint dstPos = pos;
			if(curse == 0)
			{
				dstPos = pos;
			} else if(curse == 1) {
				dstPos = iPoint(pos.x - 1, pos.y - 1);
			} else if(curse == 2) {
				dstPos = iPoint(pos.x, pos.y - 1);
			} else if(curse == 3) {
				dstPos = iPoint(pos.x + 1, pos.y - 1);
			} else if(curse == 4) {
				dstPos = iPoint(pos.x - 1, pos.y);
			} else if(curse == 5) {
				dstPos = iPoint(pos.x + 1, pos.y);
			} else if(curse == 6) {
				dstPos = iPoint(pos.x - 1, pos.y + 1);
			} else if(curse == 7) {
				dstPos = iPoint(pos.x, pos.y + 1);
			} else if(curse == 8) {
				dstPos = iPoint(pos.x + 1, pos.y + 1);
			}
	
			Cell celldeb = mCells[dstPos.y][dstPos.x];
			if(	!(mCells[dstPos.y][dstPos.x].HumanIn())
				&& (mCells[dstPos.y][dstPos.x].GetType() == Cell::CT_FLOOR 
					|| mCells[dstPos.y][dstPos.x].GetType() == Cell::CT_EXIT 
					|| mCells[dstPos.y][dstPos.x].GetType() == Cell::CT_STAIR) )
			{
				human->MoveTo(dstPos);

			} else {
				human->MoveTo(pos);
			}
	
			if(mCells[human->GetPosition().y][human->GetPosition().x].GetShortestPathLength() == 0)
			{
				human->SetOut(true);
			}

			iter++;
		}
	}
}

void Floor::EvacuateOneStepWisely()
{
	std::vector<Human*>::iterator iter = mHumans.begin();
	while(iter != mHumans.end())
	{
		Human* human = *iter;

		if(human->IsOut()){
			if(!mBaseFloor){
				mOutHumans.push_back(human);
				mCells[human->GetPosition().y][human->GetPosition().x].SetHumanIn(false);
				iter = mHumans.erase(iter);
			}
			else if(mBaseFloor->IntersectionAvailabel(human->GetPosition()))
			{
				mCells[human->GetPosition().y][human->GetPosition().x].SetHumanIn(false);
				mBaseFloor->HumanComeDown(human);
				iter = mHumans.erase(iter);
			}
			else
			{
				human->MoveTo(human->GetPosition());
				iter++;
			}

			continue;
		}
		else
		{
			iPoint pos = human->GetPosition();
			long len[9];
			len[0] = mCells[pos.y][pos.x].GetShortestPathLength();
			len[1] = mCells[pos.y - 1][pos.x - 1].HumanIn()?LONG_MAX:mCells[pos.y - 1][pos.x - 1].GetShortestPathLength();
			len[2] = mCells[pos.y - 1][pos.x].HumanIn()?LONG_MAX: mCells[pos.y - 1][pos.x].GetShortestPathLength();
			len[3] = mCells[pos.y - 1][pos.x + 1].HumanIn()?LONG_MAX: mCells[pos.y - 1][pos.x + 1].GetShortestPathLength();
			len[4] = mCells[pos.y][pos.x - 1].HumanIn()?LONG_MAX:mCells[pos.y][pos.x - 1].GetShortestPathLength();
			len[5] = mCells[pos.y][pos.x + 1].HumanIn()?LONG_MAX:mCells[pos.y][pos.x + 1].GetShortestPathLength();
			len[6] = mCells[pos.y + 1][pos.x - 1].HumanIn()?LONG_MAX:mCells[pos.y + 1][pos.x - 1].GetShortestPathLength();
			len[7] = mCells[pos.y + 1][pos.x].HumanIn()?LONG_MAX:mCells[pos.y + 1][pos.x].GetShortestPathLength();
			len[8] = mCells[pos.y + 1][pos.x + 1].HumanIn()?LONG_MAX:mCells[pos.y + 1][pos.x + 1].GetShortestPathLength();
			int min = len[0];
			/*for(int i = 0; i < 9; i++)
			{
				if(min > len[i]) 
				{
					min = len[i];
				}
			}*/

			std::vector<int> minPos;
			for(int i = 0; i < 9; i++)
			{
				if(min >= len[i])
				{
					for(int w = (min - len[i]) * 10; w >=0; w--)
					{
						minPos.push_back(i);
					}
				}
			}
	
			int i = rand() %  minPos.size();
			int curse = minPos[i];
			iPoint dstPos = pos;
			if(curse == 0)
			{
				dstPos = pos;
			} else if(curse == 1) {
				dstPos = iPoint(pos.x - 1, pos.y - 1);
			} else if(curse == 2) {
				dstPos = iPoint(pos.x, pos.y - 1);
			} else if(curse == 3) {
				dstPos = iPoint(pos.x + 1, pos.y - 1);
			} else if(curse == 4) {
				dstPos = iPoint(pos.x - 1, pos.y);
			} else if(curse == 5) {
				dstPos = iPoint(pos.x + 1, pos.y);
			} else if(curse == 6) {
				dstPos = iPoint(pos.x - 1, pos.y + 1);
			} else if(curse == 7) {
				dstPos = iPoint(pos.x, pos.y + 1);
			} else if(curse == 8) {
				dstPos = iPoint(pos.x + 1, pos.y + 1);
			}
	
			Cell celldeb = mCells[dstPos.y][dstPos.x];
			if(	!(mCells[dstPos.y][dstPos.x].HumanIn())
				&& (mCells[dstPos.y][dstPos.x].GetType() == Cell::CT_FLOOR 
					|| mCells[dstPos.y][dstPos.x].GetType() == Cell::CT_EXIT 
					|| mCells[dstPos.y][dstPos.x].GetType() == Cell::CT_STAIR) )
			{
				human->MoveTo(dstPos);

			} else {
				human->MoveTo(pos);
			}
	
			if(mCells[human->GetPosition().y][human->GetPosition().x].GetShortestPathLength() == 0)
			{
				human->SetOut(true);
			}

			iter++;
		}
	}
}

void Floor::LogPath()
{
	std::ofstream humanPaths("humanpaths.data");
	for(std::vector<Human*>::iterator iter = mOutHumans.begin(); iter != mOutHumans.end(); iter++)
	{
		(*iter)->LogPath(humanPaths);
	}
}