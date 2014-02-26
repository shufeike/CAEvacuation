#include "Floor.h"
#include "Cell.h"
#include <fstream>

Floor::Floor()
	:mCells(0), mWidth(0), mLength(0), mHeight(0)
{}

Floor::Floor(std::string file)
	:mCells(0), mWidth(0), mLength(0), mHeight(0)
{
	Load(file);
}
void Floor::Load(std::string file)
{
	try
	{
		std::ifstream terrain(file);
		float width, length;
		long id;
		float pw, pl, ph;
		int type;

		terrain>>width, length;
		mCells = new Cell*[(int) length];
		for(int i = 0; i < length; i++)
		{
			mCells[i] = new Cell[(int)width];
		}

		while(!terrain.eof())
		{
			terrain >> id >> pw >> pl >> ph >> type;
			mCells[(int)pl][(int)pw].init(id, pw, pl, ph, (Cell::CellType)type);
		}

		terrain.close();
		
		mWidth = width;
		mLength = length;

	} 
	catch(std::exception e) 
	{
		exit(-1);
	}

}


Floor::~Floor(void)
{
	if(mCells)
	{
		for(int i = 0; i < mLength; i++)
		{
			delete[] mCells[i];
		}
		
		delete[] mCells;
	}
}
