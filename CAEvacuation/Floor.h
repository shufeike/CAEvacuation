#pragma once
#include <string>
class Cell;
class Floor
{
public:
	Floor();
	Floor(std::string file);
	void Load(std::string file);
	~Floor(void);
private:
	int mWidth, mLength, mHeight;
	Cell ** mCells;
};

