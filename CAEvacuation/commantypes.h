#pragma once
typedef struct fPoint
{
	fPoint(float _x = 0, float _y = 0)
		:x(_x), y(_y){}
	float x, y;
}fPoint;

typedef struct iPoint
{
	iPoint(int _x = 0, int _y = 0, int _z = 0)
		:x(_x), y(_y), z(_z){}
	int x, y, z;
}iPoint;