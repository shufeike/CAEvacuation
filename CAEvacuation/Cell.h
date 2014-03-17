#pragma once
class Cell
{
public:
	enum CellType{
		CT_BLANK = 0, 
		CT_DC = 1, //desk and chair
		CT_WALL = 2, //wall
		CT_FLOOR = 3, //floor
		CT_PARAPET = 4, //parapet
		CT_STAIR = 5,
		CT_EXIT = 6,
		CT_INTERSECTION = 7,
	};
public:
	Cell(void);
	Cell(CellType type);
	void init(int id, float pw, float pl, float ph, CellType type);
	~Cell(void);
	CellType GetType();
	void SetShortestPathLength(long length);
	long GetShortestPathLength();

	bool HumanIn(){return humanIn;};
	void SetHumanIn(bool val){humanIn = val;};

	int GetHeight();
	int GetPosX();
	int GetPosY();
private:
	bool humanIn;
	long stepsToExit;
	CellType mCellType;
	int mPosX;
	int mPosY;
	int mHeight;
};

