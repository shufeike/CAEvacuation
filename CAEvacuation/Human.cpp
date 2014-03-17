#include "Human.h"
#include "Cell.h"
#include <vector>
#include <cstdlib>
#include <ctime>

long Human::HumanNum = 0;

Human::Human(void)
{
	cells = 0;
	out = false;
	id = ++HumanNum;
}


Human::~Human(void)
{
	HumanNum--;
}

void Human::SetCells(Cell** cs){
	cells = cs;
}

long Human::GetId()
{
	return id;
}

void Human::MoveTo(iPoint pos)
{
	cells[mCurrentPos.y][mCurrentPos.x].SetHumanIn(false); 
	cells[pos.y][pos.x].SetHumanIn(true);
	mCurrentPos = pos;
	mCurrentPos.z = cells[pos.y][pos.x].GetHeight();
	path.push_back(mCurrentPos);
}

void Human::SetPosition(iPoint pos)
{
	mCurrentPos = pos;
}

iPoint Human::GetPosition()
{
	return mCurrentPos;
}

void Human::SetOut(bool var)
{
	out = var;
}

void Human::LogPath(std::ofstream& ofs)
{
	ofs<<path.size()<<"\t\t";

	for(std::list<iPoint>::iterator iter = path.begin(); iter != path.end(); iter++)
	{
		ofs<<iter->x<<" "<<iter->y <<" "<<iter->z<<"\t\t";
	}
	ofs<<"\n";
}

void Human::TakeOneStep()
{
	if(out){
		MoveTo(mCurrentPos);
		return;
	}
	long len[9];
	len[0] = cells[mCurrentPos.y][mCurrentPos.x].GetShortestPathLength();
	len[1] = cells[mCurrentPos.y - 1][mCurrentPos.x - 1].GetShortestPathLength();
	len[2] = cells[mCurrentPos.y - 1][mCurrentPos.x].GetShortestPathLength();
	len[3] = cells[mCurrentPos.y - 1][mCurrentPos.x + 1].GetShortestPathLength();
	len[4] = cells[mCurrentPos.y][mCurrentPos.x - 1].GetShortestPathLength();
	len[5] = cells[mCurrentPos.y][mCurrentPos.x + 1].GetShortestPathLength();
	len[6] = cells[mCurrentPos.y + 1][mCurrentPos.x - 1].GetShortestPathLength();
	len[7] = cells[mCurrentPos.y + 1][mCurrentPos.x].GetShortestPathLength();
	len[8] = cells[mCurrentPos.y + 1][mCurrentPos.x + 1].GetShortestPathLength();
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
			//for(int j = 0; j <= min - len[i]; j++)
			{
				minPos.push_back(i);
			}
		}
	}
	
	
	int i = rand() %  minPos.size();
	int pos = minPos[i];
	iPoint dstPos = mCurrentPos;
	if(pos == 0)
	{
		dstPos = mCurrentPos;
	} else if(pos == 1) {
		dstPos = iPoint(mCurrentPos.x - 1, mCurrentPos.y - 1);
	} else if(pos == 2) {
		dstPos = iPoint(mCurrentPos.x, mCurrentPos.y - 1);
	} else if(pos == 3) {
		dstPos = iPoint(mCurrentPos.x + 1, mCurrentPos.y - 1);
	} else if(pos == 4) {
		dstPos = iPoint(mCurrentPos.x - 1, mCurrentPos.y);
	} else if(pos == 5) {
		dstPos = iPoint(mCurrentPos.x + 1, mCurrentPos.y);
	} else if(pos == 6) {
		dstPos = iPoint(mCurrentPos.x - 1, mCurrentPos.y + 1);
	} else if(pos == 7) {
		dstPos = iPoint(mCurrentPos.x, mCurrentPos.y + 1);
	} else if(pos == 8) {
		dstPos = iPoint(mCurrentPos.x + 1, mCurrentPos.y + 1);
	}
	
	Cell celldeb = cells[dstPos.y][dstPos.x];
	if(	!(cells[dstPos.y][dstPos.x].HumanIn())
		&& (cells[dstPos.y][dstPos.x].GetType() == Cell::CT_FLOOR 
			|| cells[dstPos.y][dstPos.x].GetType() == Cell::CT_EXIT 
			|| cells[dstPos.y][dstPos.x].GetType() == Cell::CT_STAIR) )
	{
		//cells[mCurrentPos.y][mCurrentPos.x].SetHumanIn(false);
		MoveTo(dstPos);
		//cells[dstPos.y][dstPos.x].SetHumanIn(true); 

	} else {
		MoveTo(mCurrentPos);
	}
	
	if(cells[mCurrentPos.y][mCurrentPos.x].GetShortestPathLength() == 0)
	{
		//cell[mCurrentPos.y][mCurrentPos.x].SetHumanIn(false);
		out = true;
	}
}
