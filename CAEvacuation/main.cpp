#include <fstream>
#include "Floor.h"
#define FLOOR_NUM 5
int main()
{
	Floor floors[FLOOR_NUM];
	floors[0].Load("base.data");
	return 0;
}