#pragma once
#include <string>
#include <vector>
#include <map>
#include "commantypes.h"
class ClassRoom;
class Cell;
class Human;
class Floor
{
public:
	Floor();
	Floor(std::string file);
	~Floor(void);
	void LoadCells(std::string cellFile);
	void SetupCells();
	void LoadClassRooms(std::string classroomsFile);
	void LoadClassSchedules(std::string scheduleFile);
	void DistributeHumans();
	void SetHeight(int height);
	int GetHeight();
	void SetBaseFloor(Floor* floor);
	bool IsClear();

	void EvacuateOneStep();
	void EvacuateOneStepWisely();
	bool HumanComeDown(Human* human);
	bool IntersectionAvailabel(const iPoint& pos);

	void LogPath();


private:
	int mWidth, mLength, mHeight;
	Cell ** mCells;
	//std::vector<ClassRoom*> mClassrooms;
	Floor* mBaseFloor;
	std::vector<iPoint> exits;
	std::map<int, ClassRoom*> mClassrooms;
	std::vector<Human*> mHumans;
	std::vector<Human*> mOutHumans;
};

