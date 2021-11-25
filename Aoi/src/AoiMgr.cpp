#include "AoiMgr.h"
#include "CustomAlgorithm.h"
#include <string.h>
#include <iostream>
#include <cassert>
#include <bitset>
#include "Circle.h"
using namespace std;

#define Inside_Circle -1 //在圆内
#define Intersect_Circle 0 //相交
#define Outside_Circle 1 //在圆外

uint16_t CalcFlagIndex(uint64_t flag)
{
	uint32_t flagIndex;
	if (AlgorithmUtil::IsPowerOf2(flag))
	{
		flagIndex = AlgorithmUtil::GetLog2(flag);
		if (flagIndex > Complex_Flag_Type)
			flagIndex = Complex_Flag_Type;
	}
	else
		flagIndex = Complex_Flag_Type;
	return flagIndex;
}

AoiPoint::AoiPoint(uint64_t flag, Point2D pos): pos(pos),flag(flag),pGrid(nullptr),uid(0)
{
	flagType = CalcFlagIndex(flag);
	INIT_LIST_HEAD(&head);
}
AoiPoint::~AoiPoint()
{
	if(!list_empty(&head))
		list_del_init(&head);

	pGrid->pMgr->pointMap.DelData(uid);
	std::cout << str_format("AoiPoint Destruct... gridKey:%d  pos:(%f,%f)", pGrid->key.key, pos.x, pos.y) << std::endl;
}

void AoiPoint::SetPos(Point2D pos)
{
	if (this->pos == pos)
		return;
	this->pos = pos;
	pGrid->pMgr->AddAoiPoint(*this);
}

uint64_t AoiPoint::AddTrigger(uint64_t flag, float enterDis, float exitDis)
{
	AoiTrigger* pTrigger = new AoiTrigger(flag,enterDis,exitDis);
	pTrigger->pMgr = pGrid->pMgr;
	uint64_t triggerId = pGrid->pMgr->triggerMap.AddData(pTrigger);
	triggerList.push_back(triggerId);
	return triggerId;
}

void AoiPoint::UpdateTriggers()
{
	for (auto triggerId : triggerList)
	{
		AoiTrigger* pTrigger = pGrid->pMgr->triggerMap.GetData(triggerId);
		if (pTrigger)
			pTrigger->pos = pos;
			pTrigger->Update();
	}
}

AoiGrid::AoiGrid(const GridKey& key,AoiMgr* mgr):key(key),pMgr(mgr)
{
	for (int i=0;i <= Complex_Flag_Type;i++)
		INIT_LIST_HEAD(flagHead+i);
	
	box.topLeft = Point2D(key.x, key.y)*pMgr->gridSize;
	box.bottomRight = box.topLeft + Point2D(pMgr->gridSize, pMgr->gridSize);
}


AoiGrid::~AoiGrid()
{
	AoiPoint *point, *n;
	for (int i = 0; i <= Complex_Flag_Type; i++)
	{
		// list_for_each_entry_safe 遍历时安全删除
		list_for_each_entry_safe(point, n, flagHead + i, head)
		{
			SAFE_DELETE(point);
		}
	}

	pMgr->gridMap.erase(key.key);
	std::cout << str_format("AoiGrid Destruct... gridKey:%d  center:(%d,%d)",key.key, key.x, key.y) << std::endl;
}

void AoiGrid::AddPoint(AoiPoint& point)
{
	if (point.pGrid == this)
		return;

	if (point.flagType == Complex_Flag_Type)
	{
		complexFlag |= point.flag;
	}
	else
	{
		singleFlag |= point.flag;
	}

	point.pGrid = this;
	list_move(&point.head, flagHead + point.flagType);

	std::cout << str_format("AoiGrid AddPoint center:(%d,%d)	point:(%f,%f)", key.x, key.y,point.pos.x,point.pos.y)<< std::endl;
}

void AoiGrid::DelPoint(AoiPoint& point)
{
	if (list_empty(flagHead + point.flagType))
	{
		if (point.flagType == Complex_Flag_Type)
			complexFlag = 0;
		else
			singleFlag ^= point.flagType;
	}
	std::cout << str_format("AoiGrid DelPoint center:(%d,%d)	point:(%f,%f)", key.x, key.y, point.pos.x, point.pos.y) << std::endl;
}

void AoiGrid::Debug()
{
	AoiPoint* p;
	
	for (int i = 0; i <= Complex_Flag_Type; ++i)
	{
		uint32_t count = 0;
		list_for_each_entry(p, flagHead + i, head)
		{
			count++;
		}
		if (count > 0)
			std::cout << str_format("grid:(%d,%d) index:%d count:%d",key.x,key.y,i,count) << std::endl;
	}
}

void AoiGrid::ForeachPoint(FlagFilter filter, ForeachPointFun fun, void* args)
{

	AoiPoint* p;
	if (singleFlag & filter.flag)
	{
		for (auto ft : filter.flagTypeList)
		{
			list_for_each_entry(p, flagHead + ft, head)
			{
				fun(p, args);
			}
		}
	}

	if (complexFlag & filter.flag)
	{
		list_for_each_entry(p, flagHead + Complex_Flag_Type, head)
		{
			if(p->flag & filter.flag)
				fun(p,args);
		}
	}
}

void AoiGrid::UpdateTrigger()
{

}
/*************************** AoiTrigger ****************************************/
FlagFilter::FlagFilter(uint64_t flag)
{
	bitset<Complex_Flag_Type> bs(flag);
	for (int i = 0; i < Complex_Flag_Type; i++)
	{
		if (bs[i])
			flagTypeList.push_back(i);
	}

	this->flag = flag;
}



AoiTrigger::AoiTrigger(uint64_t flag, float enterDis, float exitDis):filter(flag)
{
	this->enterDis = enterDis;
	this->exitDis = exitDis;

}

AoiTrigger::~AoiTrigger()
{

}

struct GridHandleArgs
{
	AoiTrigger* pTrigger;
	Point2D pos;
	int side; //-1 inner；0 Intersect；1 outer
};

void _GridEnterTrigger(AoiPoint* point, void* args)
{
	GridHandleArgs* pArgs = (GridHandleArgs*)args;
	if (pArgs->side == Inside_Circle)
		pArgs->pTrigger->pointSet.insert(point->uid);
	else if (pArgs->side == Intersect_Circle)
	{
		if (Point2D::Distance(point->pos, pArgs->pos) <= pArgs->pTrigger->enterDis)
			pArgs->pTrigger->pointSet.insert(point->uid);
	}
}

void _GridExitTrigger(AoiPoint* point, void* args)
{
	GridHandleArgs* pArgs = (GridHandleArgs*)args;
	if (pArgs->side == Outside_Circle)
		pArgs->pTrigger->pointSet.erase(point->uid);
	else if (pArgs->side == Intersect_Circle)
	{
		if (Point2D::Distance(point->pos, pArgs->pos) >= pArgs->pTrigger->exitDis)
			pArgs->pTrigger->pointSet.erase(point->uid);
	}
}

void AoiTrigger::Update()
{
	UpdateGrid();
}

void AoiTrigger::UpdateGrid()
{
	Circle enterCircle = Circle(pos, enterDis);
	Rectangle enterRect = enterCircle.GetOutSideRect();
	
	//以pos为圆心，enterDis为半径，圆的外切矩形
	GridKey gMin = pMgr->CalcGridKey(enterRect.topLeft);
	GridKey gMax = pMgr->CalcGridKey(enterRect.bottomRight);

	for (int x = gMin.x; x <= gMax.x; x++)
	{
		for (int y = gMin.y; y <= gMax.y; y++)
		{
			uint64_t key = GENUUID64(x, y);
			AoiGrid* pGrid = pMgr->gridMap[key];
			if (pGrid == nullptr)
				continue;

			int oldContain = pGrid->containMap[this->uid];

			int newSide = enterCircle.CheckPosionalSide(pGrid->box);

			pGrid->containMap[this->uid] = newSide;

			if (gridSet.find(key) == gridSet.end())
			{
				gridSet.emplace(key);

				GridHandleArgs args = { this,this->pos,newSide};
				pGrid->ForeachPoint(filter, _GridEnterTrigger, (void*)&args);
				continue;
			}

			//// 上次跟本次都在圆内，不处理
			//if (newSide == Inside_Circle && oldContain == Inside_Circle)
			//{
			//	continue;
			//}

			//GridHandleArgs args = { this,this->pos,newSide };
			//pGrid->ForeachPoint(filter, _GridEnterTrigger, (void*)&args);
		}
	}

	Circle exitCircle = Circle(pos, exitDis);
	Rectangle exitRect = exitCircle.GetOutSideRect();
	std::set<uint64_t> needRemoveGrids;
	for (auto key:gridSet)
	{
		AoiGrid* pGrid = pMgr->gridMap[key];
		if (pGrid == nullptr)
			continue;

		// 上次跟本次都在圆内，不处理
		int enterSide = enterCircle.CheckPosionalSide(pGrid->box);
		if (enterSide == Inside_Circle)
		{
			continue;
		}

		if(enterSide == Intersect_Circle)
		{ 
			GridHandleArgs args = { this,this->pos,enterSide};
			pGrid->ForeachPoint(filter, _GridEnterTrigger, (void*)&args);
		}


		int side = exitCircle.CheckPosionalSide(pGrid->box);
		if (side == Outside_Circle)
		{
			needRemoveGrids.insert(key);
		}
		if (side != Inside_Circle)
		{
			GridHandleArgs args = { this,this->pos,side};
			pGrid->ForeachPoint(filter, _GridExitTrigger, (void*)&args);
		}
		
	}

	for (auto key : needRemoveGrids)
	{
		AoiGrid* pGrid = pMgr->gridMap[key];
		if (pGrid == nullptr)
			continue;
		pGrid->containMap.erase(this->uid);
		gridSet.erase(key);

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

GridKey AoiMgr::CalcGridKey(const Point2D& pos)
{
	uint32_t x = uint32_t(pos.x / gridSize);
	uint32_t y = uint32_t(pos.y / gridSize);
	return GridKey(x,y);
}

void AoiMgr::AddAoiPoint(AoiPoint& point)
{
	// 第一次添加
	if (point.uid == 0)
		point.uid = pointMap.AddData(&point);

	// 仍然在同一个grid中
	GridKey gridCenter = CalcGridKey(point.pos);
	if (point.pGrid && point.pGrid->key == gridCenter)
		return;
	// 所在grid发生变化
	AoiGrid* pOldGrid = point.pGrid;
	AoiGrid* pGrid = AddGrid(gridCenter);
	pGrid->AddPoint(point);
	if(pOldGrid != nullptr)
		pOldGrid->DelPoint(point);

}

AoiGrid* AoiMgr::AddGrid(const GridKey& gridKey)
{
	AoiGrid* pGrid = gridMap[gridKey.key];
	if (pGrid == nullptr)
	{
		pGrid = new AoiGrid(gridKey,this);
		assert(pGrid);
		gridMap[gridKey.key] = pGrid;
	}
		
	return pGrid;
}

