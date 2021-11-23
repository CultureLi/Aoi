#include "AoiMgr.h"
#include "CustomAlgorithm.h"
#include <string.h>
#include <iostream>
#include <cassert>
using namespace std;

AoiPoint::AoiPoint(uint64_t flag, Point2D pos): pos(pos),flag(flag),pGrid(nullptr),uid(0)
{
	if (AlgorithmUtil::IsPowerOf2(flag))
	{
		flagHeadIndex = AlgorithmUtil::GetLog2(flag);
		if (flagHeadIndex > Complex_Flag)
			flagHeadIndex = Complex_Flag;
	}
	else
		flagHeadIndex = Complex_Flag;

	INIT_LIST_HEAD(&head);
}
AoiPoint::~AoiPoint()
{
	if(!list_empty(&head))
		list_del_init(&head);

	pGrid->pMgr->pointMap.DelData(uid);
	std::cout << str_format("AoiPoint Destruct... gridKey:%d  pos:(%f,%f)", pGrid->center.key, pos.x, pos.y) << std::endl;
}

void AoiPoint::SetPos(Point2D pos)
{
	if (this->pos == pos)
		return;
	this->pos = pos;
	pGrid->pMgr->AddAoiPoint(*this);
}


AoiGrid::AoiGrid(const GridCenter& center):center(center),pMgr(nullptr)
{
	for (int i=0;i <= Complex_Flag;i++)
		INIT_LIST_HEAD(flagHead+i);

}


AoiGrid::~AoiGrid()
{
	AoiPoint *point, *n;
	for (int i = 0; i <= Complex_Flag; i++)
	{
		// list_for_each_entry_safe 遍历时安全删除
		list_for_each_entry_safe(point, n, flagHead + i, head)
		{
			SAFE_DELETE(point);
		}
	}

	pMgr->gridMap.erase(center.key);
	std::cout << str_format("AoiGrid Destruct... gridKey:%d  center:(%d,%d)",center.key, center.x, center.y) << std::endl;
}

void AoiGrid::AddPoint(AoiPoint& point)
{
	if (point.pGrid == this)
		return;

	point.pGrid = this;
	list_move(&point.head, flagHead + point.flagHeadIndex);

	std::cout << str_format("AoiGrid AddPoint center:(%d,%d)	point:(%f,%f)", center.x, center.y,point.pos.x,point.pos.y)<< std::endl;
}

void AoiGrid::DelPoint(AoiPoint& point)
{
	std::cout << str_format("AoiGrid DelPoint center:(%d,%d)	point:(%f,%f)", center.x, center.y, point.pos.x, point.pos.y) << std::endl;
}

void AoiGrid::Debug()
{
	AoiPoint* p;
	
	for (int i = 0; i <= Complex_Flag; ++i)
	{
		uint32_t count = 0;
		list_for_each_entry(p, flagHead + i, head)
		{
			count++;
		}
		if (count > 0)
			std::cout << str_format("grid:(%d,%d) index:%d count:%d",center.x,center.y,i,count) << std::endl;
	}
}




/*************************** AoiMgr ****************************************/

AoiMgr::AoiMgr(Point2D mapSize, uint32_t gridSize)
{
	if (mapSize.x / gridSize > maxGridsCount ||
		mapSize.y / gridSize > maxGridsCount)
	{
		throw("Aoi gridSize too small");
	}
	this->mapSize = mapSize;
	this->gridSize = gridSize;
}

AoiMgr::~AoiMgr()
{
	//删除所有 grid -> 删除所有 AoiPoint
	auto keylist = extract_keys<uint64_t, AoiGrid*>(gridMap);
	for (auto uid : keylist)
	{
		SAFE_DELETE(gridMap[uid]);
	}
	cout << "AoiMgr Destruct ..." << endl;
}

GridCenter AoiMgr::CalcGridCenter(const Point2D& pos)
{
	uint32_t x = uint32_t(pos.x / gridSize) * gridSize + gridSize/2;
	uint32_t y = uint32_t(pos.y / gridSize) * gridSize + gridSize/2;
	return GridCenter(x,y);
}

void AoiMgr::AddAoiPoint(AoiPoint& point)
{
	// 第一次添加
	if (point.uid == 0)
		point.uid = pointMap.AddData(&point);

	// 仍然在同一个grid中
	GridCenter gridCenter = CalcGridCenter(point.pos);
	if (point.pGrid && point.pGrid->center == gridCenter)
		return;
	// 所在grid发生变化
	AoiGrid* pOldGrid = point.pGrid;
	AoiGrid* pGrid = AddGrid(gridCenter);
	pGrid->AddPoint(point);
	if(pOldGrid != nullptr)
		pOldGrid->DelPoint(point);

}

AoiGrid* AoiMgr::AddGrid(const GridCenter& center)
{
	AoiGrid* pGrid = gridMap[center.key];
	if (pGrid == nullptr)
	{
		pGrid = new AoiGrid(center);
		assert(pGrid);
		gridMap[center.key] = pGrid;
		pGrid->pMgr = this;
	}
		
	return pGrid;
}

