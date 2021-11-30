#include "AoiMgr.h"
#include "CustomAlgorithm.h"
#include <string.h>
#include <iostream>
#include <cassert>
#include <bitset>
#include "Circle.h"
using namespace std;

void HandlePointInTrigger(AoiPoint* point, void* args)
{
	GridHandleArgs* pArgs = (GridHandleArgs*)args;
	int action = 0;
	if (pArgs->enterSide == EPosionalType::E_Inside)
	{
		action = 1;
	}
	else if (pArgs->exitSide == EPosionalType::E_Outside)
	{
		action = -1;
	}
	else if (pArgs->enterSide == EPosionalType::E_Intersect
		|| pArgs->exitSide == EPosionalType::E_Intersect)
	{
		float distance = Point2D::Distance(point->pos, pArgs->pTrigger->pos);
		if (distance < pArgs->pTrigger->enterDis)
			action = 1;
		else if (distance > pArgs->pTrigger->exitDis)
			action = -1;
	}

	auto bHas = (pArgs->pTrigger->pointSet.find(point->uid) != pArgs->pTrigger->pointSet.end());
	if (action == 1)
	{
		if (!bHas)
		{
			pArgs->pTrigger->pointSet.insert(point->uid);
			pArgs->pTrigger->EnterTriggerCallBack(point);
		}
	}
	else if (action == -1)
	{
		if (bHas)
		{
			pArgs->pTrigger->pointSet.erase(point->uid);
			pArgs->pTrigger->ExitTriggerCallBack(point);
		}
	}


}

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

AoiPoint::AoiPoint(AoiMgr* pMgr, uint64_t flag, Point2D pos): pos(pos),flag(flag),pGrid(nullptr)
{
	assert(pMgr);
	this->uid = pMgr->pointMap.AddData(this);
	flagType = CalcFlagIndex(flag);
	INIT_LIST_HEAD(&head);
}
AoiPoint::~AoiPoint()
{
	if(!list_empty(&head))
		list_del_init(&head);
	if(pGrid)
		pGrid->DelPoint(this);
	pGrid = nullptr;
	//std::cout << str_format("AoiPoint Destruct... gridKey:%d  pos:(%f,%f)", pGrid->key.key, pos.x, pos.y) << std::endl;
}

void AoiPoint::SetPos(Point2D pos)
{
	if (this->pos == pos)
		return;
	this->pos = pos;

	pGrid->pMgr->AddAoiPoint(this);
}

void AoiPoint::BindTrigger(uint32_t triggerId)
{
	triggerSet.emplace(triggerId);
}

void AoiPoint::UpdateTriggers()
{
	for (auto triggerId : triggerSet)
	{
		AoiTrigger* pTrigger = pGrid->pMgr->triggerMap.GetData(triggerId);
		if (pTrigger)
		{
			if (pTrigger->pos == pos)
			{
				//continue;
			}
			pTrigger->pos = pos;
			pTrigger->Update();
		}
	}
}

AoiGrid::AoiGrid(const GridKey& key,AoiMgr* mgr):key(key),pMgr(mgr)
{
	for (int i=0;i <= Complex_Flag_Type;i++)
		INIT_LIST_HEAD(flagHead+i);
	
	box.topLeft = Point2D((float)key.x, (float)key.y)* (float)pMgr->gridSize;
	box.bottomRight = box.topLeft + Point2D((float)pMgr->gridSize, (float)pMgr->gridSize);
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


void AoiGrid::ForeachTrigger(AoiPoint* point)
{
	
	for (auto iter : triggerSideMap)
	{
		AoiTrigger* pTrigger = pMgr->triggerMap.GetData(iter.first);
		if (pTrigger == nullptr)
			continue;

		if ((pTrigger->filter.flag & point->flag) == 0)
			continue;


		Circle enterCircle = Circle(pTrigger->pos, pTrigger->enterDis);

		auto side = enterCircle.CheckPosionalSide(point->pGrid->box);
		GridHandleArgs args
		{
			pTrigger,
			side,
			side
		};

		HandlePointInTrigger(point, (void*)&args);
		
	}
}

void AoiGrid::AddPoint(AoiPoint* point)
{
	if (point->pGrid == this)
		return;

	if (point->flagType == Complex_Flag_Type)
	{
		complexFlag |= point->flag;
	}
	else
	{
		singleFlag |= point->flag;
	}

	point->pGrid = this;
	list_move(&point->head, flagHead + point->flagType);

	ForeachTrigger(point);

	//std::cout << str_format("AoiGrid AddPoint center:(%d,%d)	point:(%f,%f)", key.x, key.y,point.pos.x,point.pos.y)<< std::endl;
}



void AoiGrid::DelPoint(AoiPoint* point)
{
	if (list_empty(flagHead + point->flagType))
	{
		if (point->flagType == Complex_Flag_Type)
			complexFlag = 0;
		else
			singleFlag ^= point->flagType;
	}

	ForeachTrigger(point);

	//std::cout << str_format("AoiGrid DelPoint center:(%d,%d)	point:(%f,%f)", key.x, key.y, point.pos.x, point.pos.y) << std::endl;
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


AoiTrigger::AoiTrigger(uint64_t flag, float enterDis, float cacheDis, TriggerCallBack entercb, TriggerCallBack exitcb):
	filter(flag),EnterTriggerCallBack(entercb),ExitTriggerCallBack(exitcb)
{
	this->enterDis = enterDis;
	this->exitDis = enterDis+cacheDis;

}

AoiTrigger::~AoiTrigger()
{

}


void AoiTrigger::Update()
{
	Circle enterCircle = Circle(pos, enterDis);
	Rectangle enterRect = enterCircle.GetOutSideRect();

	pMgr->CalcGridsInRange(enterCircle, gridSet);

	Circle exitCircle = Circle(pos, exitDis);
	Rectangle exitRect = exitCircle.GetOutSideRect();
	std::set<uint64_t> needRemoveGrids;
	for (auto key:gridSet)
	{
		AoiGrid* pGrid = pMgr->gridMap[key];
		if (pGrid == nullptr)
			continue;

		EPosionalType oldEnterSide = pGrid->triggerSideMap[this->uid];
		EPosionalType newEnterSide = enterCircle.CheckPosionalSide(pGrid->box);
		
		pGrid->triggerSideMap[this->uid] = newEnterSide;

		if (oldEnterSide == EPosionalType::E_Inside && newEnterSide == EPosionalType::E_Inside)
		{
			continue;
		}

		EPosionalType exitside = exitCircle.CheckPosionalSide(pGrid->box);
		GridHandleArgs args = { this,newEnterSide,exitside };
		pGrid->ForeachPoint(filter, HandlePointInTrigger, (void*)&args);

		
		if (exitside == EPosionalType::E_Outside)
		{
			needRemoveGrids.insert(key);
		}
		
	}

	for (auto key : needRemoveGrids)
	{
		AoiGrid* pGrid = pMgr->gridMap[key];
		if (pGrid == nullptr)
			continue;
		pGrid->triggerSideMap.erase(this->uid);
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

AoiGrid* AoiMgr::AddGrid(const GridKey& gridKey)
{
	AoiGrid* pGrid = gridMap[gridKey.key];
	if (pGrid == nullptr)
	{
		pGrid = new AoiGrid(gridKey, this);
		assert(pGrid);
		gridMap[gridKey.key] = pGrid;
	}

	return pGrid;
}

void AoiMgr::AddAoiPoint(AoiPoint* point)
{
	// 仍然在同一个grid中
	GridKey gridKey = CalcGridKey(point->pos);
	if (point->pGrid
		&& point->pGrid->key == gridKey)
		return;
	// 所在grid发生变化
	AoiGrid* pOldGrid = point->pGrid;
	AoiGrid* pGrid = AddGrid(gridKey);
	//point->pGrid = pGrid;
	pGrid->AddPoint(point);
	if (pOldGrid != nullptr)
		pOldGrid->DelPoint(point);

	
}

uint32_t AoiMgr::CreateTrigger(uint64_t flag, float enterDis, float cacheDis, TriggerCallBack entercb, TriggerCallBack exitcb)
{
	AoiTrigger* pTrigger = new AoiTrigger(flag, enterDis, cacheDis,entercb,exitcb);
	pTrigger->pMgr = this;
	pTrigger->uid = triggerMap.AddData(pTrigger);
	return pTrigger->uid;
}


void AoiMgr::CalcGridsInRange(Circle circle, std::set<uint64_t>& gridSet)
{
	Rectangle rect = circle.GetOutSideRect();

	//以pos为圆心，圆的外切矩形
	GridKey gMin = CalcGridKey(rect.topLeft);
	GridKey gMax = CalcGridKey(rect.bottomRight);


	// 跟外切矩形相交、包含的grid
	for (uint32_t x = gMin.x; x <= gMax.x; x++)
	{
		for (uint32_t y = gMin.y; y <= gMax.y; y++)
		{
			uint64_t key = GENUUID64(x, y);
			auto iter = gridMap.find(key);
			if (iter == gridMap.end())
				continue;

			AoiGrid* pGrid = iter->second;
			if (gridSet.find(key) == gridSet.end())
			{
				int newSide = circle.CheckPosionalSide(pGrid->box);
				if (newSide != EPosionalType::E_Outside)
					gridSet.emplace(key);
			}
		}
	}
}


struct CollectPointArgs
{
	AoiGrid* pGrid;
	Circle &circle;
	EPosionalType side;
	std::list<AoiPoint*>& lst;
};
void CollectPoint(AoiPoint* point,void* args)
{
	CollectPointArgs* pArgs = (CollectPointArgs*)args;
	switch (pArgs->side)
	{
		case EPosionalType::E_Inside:
			pArgs->lst.push_back(point);
			break;
		case EPosionalType::E_Intersect:
		{
			if(pArgs->circle.Contain(point->pos))
				pArgs->lst.push_back(point);
		}
		break;
	}
}


class DistanceLessSorter
{
public:
	DistanceLessSorter(Point2D c) :center(c) {};
	Point2D center;

	bool operator () (const AoiPoint* p1, const AoiPoint* p2)
	{
		float dis1 = Point2D::DistanceSqrt(center, p1->pos);
		float dis2 = Point2D::DistanceSqrt(center, p2->pos);
		if (dis1 == dis2)
			return p1->uid>p2->uid;
		else
			return dis1 < dis2;
	}
};

std::list<AoiPoint*> AoiMgr::GetNearPoints(Point2D pos,float dis, uint64_t flag,int maxCount)
{
	std::set<uint64_t> gridSet;
	Circle circle = Circle(pos, dis);
	CalcGridsInRange(circle,gridSet);
	std::list<AoiPoint*> lst;
	for (auto key : gridSet)
	{
		auto iter = gridMap.find(key);
		if (iter == gridMap.end())
			continue;

		AoiGrid* pGrid = iter->second;

		EPosionalType side = circle.CheckPosionalSide(pGrid->box);
		if (side == EPosionalType::E_Outside)
			continue;

		CollectPointArgs args{ pGrid,circle,side,lst };
		pGrid->ForeachPoint(FlagFilter(flag), CollectPoint, (void*)&args);
	}
	

	lst.sort(DistanceLessSorter(pos));
	if (maxCount > 0 && lst.size() > maxCount)
		lst.resize(maxCount);

	return lst;
}