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
	// 0 ~ Complex_Flag_Type，记录放在grid的哪个head下
	uint16_t flagType; 

	list_head head;

	AoiGrid* pGrid;
	uint64_t uid;
	Point2D pos;

	std::set<uint32_t> triggerSet; //绑定的triggerid集合
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

	// 放point的head,[0,64]放单个flag，Complex_Flag_Type放组合flag
	list_head flagHead[All_Flag_Type]; 

	uint64_t complexFlag;
	uint64_t singleFlag;

	// grid格子对应的矩形
	Custom_Develop::Rectangle box;

	//和触发器的位置关系（包含和相交,在触发器外会被移除)
	std::map<uint32_t, EPosionalType> triggerSideMap;

	AoiMgr* pMgr;
private:

	

};
/******************************************** AoiTrigger***************************************/
struct GridHandleArgs
{
	AoiTrigger* pTrigger;
	EPosionalType enterSide; //和进入圆的关系
	EPosionalType exitSide;//和离开圆的关系

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

	// 该触发器过滤哪些flag的point
	FlagFilter filter;

	// 进入、离开距离，使用一段缓冲带来避免频繁增删
	float enterDis;
	float exitDis;
	
	// 跟内圈 相交、包含关系的格子
	std::set<uint64_t> gridSet;
	//触发器内的point
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


