#include <fstream>
#include <iostream>
#include "Floor.h"
#include "CellManager.h"
#include "HumanManager.h"
#define FLOOR_NUM 5
#include <cstdlib>
#include <ctime>
int main()
{
	srand(time(0));
	//new CellManager();
	//new HumanManager();
	Floor floors[FLOOR_NUM];
	for(int i = 0; i < FLOOR_NUM; i++)
	{
		floors[i].SetHeight(i * 12);
		if(i == 0)
		{	
			floors[i].LoadCells("base.data");
		} 
		else if(i == FLOOR_NUM -1) 
		{
			floors[i].LoadCells("top.data");
			floors[i].SetBaseFloor(&floors[i - 1]);
		} 
		else 
		{
			floors[i].LoadCells("mid.data");
			floors[i].SetBaseFloor(&floors[i - 1]);
		}

		floors[i].LoadClassRooms("ClassRooms.xml");
		floors[i].LoadClassSchedules("ClassSchedule.xml");
		floors[i].SetupCells();
		floors[i].DistributeHumans();
	}
	bool clear = false;
	while(!clear){
		clear = true;
		for(int i = 0; i < FLOOR_NUM; i++)
		{
			if(!floors[i].IsClear())
			{
				clear = false;
				//floors[i].EvacuateOneStep();
				floors[i].EvacuateOneStepWisely();
			}
		}
	}

	floors[0].LogPath();

	std::cout<<"Press any key to continue!"<<std::endl;
	getchar();
	return 0;
}