#pragma once
#include <stdint.h>
#include "Common.h"
#include <map>
#include "Point2D.h"
#include "CIntrusiveList.h"
#include "CustomMap.h"

#define Complex_Flag 64
#define All_Flag (Complex_Flag+1)


class AoiGrid;
class AoiMgr;
/******************************************** AoiPoint ***************************************/

class AoiPoint
{
public:
	AoiPoint(uint64_t flag,Point2D pos);
	~AoiPoint();

	void SetPos(Point2D& pos);

	uint64_t flag;
	uint32_t flagHeadIndex;

	AoiGrid* pGrid;
	uint64_t uid;

	list_head head;

	Point2D pos;
};

/******************************************** AoiGrid***************************************/

struct GridCenter
{
	GridCenter() :x(0), y(0), key(0) {}
	GridCenter(uint32_t x,uint32_t y)
	{
		this->x = x;
		this->y = y;
		this->key = GENUUID64(x, y);
	}

	bool operator==(const GridCenter& other)
	{
		return (x == other.x && y == other.y);
	}

	uint32_t x;
	uint32_t y;
	uint64_t key;


};

class AoiGrid
{
public:
	AoiGrid(const GridCenter&);
	~AoiGrid();
	GridCenter center;
	void AddPoint(AoiPoint&);
	void DelPoint(AoiPoint&);

	list_head flagHead[All_Flag];

	void Debug();

	AoiMgr* pMgr;
private:

	

};

/******************************************** CTimerMgr***************************************/

class AoiMgr
{
	
public:
	AoiMgr(const Point2D& mapSize, const uint32_t gridSize);
	~AoiMgr();

	GridCenter CalcGridCenter(const Point2D& pos);

	void AddAoiPoint(AoiPoint&);
	AoiGrid* AddGrid(const GridCenter& center);

public:

private:

	uint32_t const maxGridsCount = 100;
	Point2D mapSize;
	uint32_t gridSize;

public:
	CustomMap<uint64_t, AoiPoint*, 64, 0> pointMap;
	std::map<uint64_t, AoiGrid*> gridMap;
};


