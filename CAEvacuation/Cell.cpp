#include "Cell.h"
#include <climits>

Cell::Cell(void)
{
	humanIn = false;
	stepsToExit = LONG_MAX;
	mCellType = Cell::CT_BLANK;
	mPosX = 0;
	mPosY = 0;
	mHeight = 0;
}

Cell::Cell(Cell::CellType type)
{
	mCellType = type;
	stepsToExit = LONG_MAX;
	humanIn = false;
	mPosX = 0;
	mPosY = 0;
	mHeight = 0;
}

Cell::~Cell(void)
{
}

void Cell::init(int id, float pw, float pl, float ph, CellType type)
{
	mPosX = pw;
	mPosY = pl;
	mHeight = ph;
	mCellType = type;
}

Cell::CellType Cell::GetType()
{
	return mCellType;
}

void Cell::SetShortestPathLength(long length)
{
	stepsToExit = length;
}

long Cell::GetShortestPathLength()
{
	return stepsToExit;
}


int Cell::GetHeight()
{
	return mHeight;
}

int Cell::GetPosX()
{
	return mPosX;
}

int Cell::GetPosY()
{
	return mPosY;
}
