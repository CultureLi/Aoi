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
#include "Circle.h"

using namespace Custom_Develop;

#define Complex_Flag_Type 64
#define All_Flag_Type (Complex_Flag_Type+1)

class AoiGrid;
class AoiMgr;
class AoiPoint;
class FlagFilter;
class AoiTrigger;
typedef void(*ForeachPointFun)(AoiPoint*, void*);
typedef void(*ForeachGridFun)(AoiGrid*, void*);
typedef void(*ForeachTriggerFun)(AoiTrigger*, void*);
/******************************************** AoiPoint ***************************************/

class AoiPoint
{
public:
	AoiPoint(AoiMgr*pMgr, uint64_t flag,Point2D pos);
	~AoiPoint();

	void SetPos(Point2D pos);

	void BindTrigger(uint32_t triggerId);
	void UpdateTriggers();

	uint64_t flag;
	uint16_t flagType;

	AoiGrid* pGrid;
	uint64_t uid;

	list_head head;

	Point2D pos;

	std::set<uint64_t> triggerSet;
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
	void AddPoint(AoiPoint*);
	void DelPoint(AoiPoint*);
	void ForeachPoint(FlagFilter filter, ForeachPointFun fun,void* args);
	void ForeachTrigger(AoiPoint* point);

	list_head flagHead[All_Flag_Type];

	uint64_t complexFlag;
	uint64_t singleFlag;

	Custom_Develop::Rectangle box;

	//和触发器的位置关系（包含和相交,在触发器外会被移除)
	std::map<uint32_t, EPosionalType> triggerSideMap;

	void Debug();

	AoiMgr* pMgr;
private:

	

};
/******************************************** AoiTrigger***************************************/
struct GridHandleArgs
{
	AoiTrigger* pTrigger;
	EPosionalType enterSide;
	EPosionalType exitSide;

};

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
	AoiTrigger(uint64_t flag,float enterDis,float cacheDis);
	~AoiTrigger();

	FlagFilter filter;

	float enterDis;
	float exitDis;
	
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

	void AddAoiPoint(AoiPoint*);
	AoiGrid* AddGrid(const GridKey& center);
	uint32_t CreateTrigger(uint64_t flag, float enterDis, float cacheDis);

public:
	Point2D mapSize;
	uint32_t gridSize;
private:

	uint32_t const maxGridsCount = 100;


public:
	CustomMap<uint64_t, AoiPoint*, 64, 0> pointMap;
	std::map<uint64_t, AoiGrid*> gridMap;
	CustomMap<uint32_t, AoiTrigger*, 32, 0> triggerMap;
};


