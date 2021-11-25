#pragma once
#include <stdint.h>
#include "Common.h"
#include <map>
#include <set>
#include <list>
#include "Point2D.h"
#include "CIntrusiveList.h"
#include "CustomMap.h"
#include "Rectangle.h"

using namespace Custom_Develop;

#define Complex_Flag_Type 64
#define All_Flag_Type (Complex_Flag_Type+1)

class AoiGrid;
class AoiMgr;
class AoiPoint;
class FlagFilter;
typedef void(*ForeachPointFun)(AoiPoint*, void*);
typedef void(*ForeachGridFun)(AoiGrid*, void*);
/******************************************** AoiPoint ***************************************/

class AoiPoint
{
public:
	AoiPoint(uint64_t flag,Point2D pos);
	~AoiPoint();

	void SetPos(Point2D pos);

	uint64_t AddTrigger(uint64_t flag, float enterDis, float exitDis);
	void UpdateTriggers();

	uint64_t flag;
	uint16_t flagType;

	AoiGrid* pGrid;
	uint64_t uid;

	list_head head;

	Point2D pos;

	std::list<uint64_t> triggerList;
};

/******************************************** AoiGrid***************************************/

struct GridKey
{
	GridKey() :x(0), y(0), key(0) {}
	GridKey(uint32_t x,uint32_t y)
	{
		this->x = x;
		this->y = y;
		this->key = GENUUID64(x, y);
	}

	bool operator==(const GridKey& other)
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
	AoiGrid(const GridKey&, AoiMgr* mgr);
	~AoiGrid();
	GridKey key;
	void AddPoint(AoiPoint&);
	void DelPoint(AoiPoint&);
	void UpdateTrigger();
	void ForeachPoint(FlagFilter filter, ForeachPointFun fun,void* args);

	list_head flagHead[All_Flag_Type];

	uint64_t complexFlag;
	uint64_t singleFlag;

	Custom_Develop::Rectangle box;

	//´¥·¢Æ÷ÊÇ·ñ°üº¬
	std::map<uint32_t, int> containMap; 

	void Debug();

	AoiMgr* pMgr;
private:

	

};
/******************************************** AoiTrigger***************************************/
class FlagFilter
{
public:
	FlagFilter(uint64_t flag);

	uint64_t flag;
	std::list<uint16_t> flagTypeList;
};

class AoiTrigger
{
public:
	AoiTrigger(uint64_t flag,float enterDis,float exitDis);
	~AoiTrigger();

	FlagFilter filter;

	float enterDis;
	float exitDis;

	Custom_Develop::Rectangle enterRect;
	Custom_Develop::Rectangle exitRect;

	
	std::set<uint64_t> gridSet;
	std::set<uint64_t> pointSet;
	
	

	AoiMgr* pMgr;

	uint32_t uid;
	Point2D pos;

	void Update();
	void UpdateGrid();
	void UpdatePoint();

};

/******************************************** AoiMgr***************************************/

class AoiMgr
{
	
public:
	AoiMgr(Point2D mapSize, uint32_t gridSize);
	~AoiMgr();

	GridKey CalcGridKey(const Point2D& pos);

	void AddAoiPoint(AoiPoint&);
	AoiGrid* AddGrid(const GridKey& center);

public:
	Point2D mapSize;
	uint32_t gridSize;
private:

	uint32_t const maxGridsCount = 100;


public:
	CustomMap<uint64_t, AoiPoint*, 64, 0> pointMap;
	std::map<uint64_t, AoiGrid*> gridMap;
	CustomMap<uint64_t, AoiTrigger*, 32, 0> triggerMap;
};


