
#include<iostream>
#include <graphics.h>		// 引用图形库头文件
#include <conio.h>

#include "Point2D.h"
#include "AoiMgr.h"

Point2D mapSize = Point2D(1024, 1024);


uint32_t gridSize = 100; //格子大小
uint32_t halfSize = gridSize / 2;

AoiPoint* controlPoint;
AoiPoint* controlPoint2;
Point2D mousePos;

void Draw(AoiMgr* mgr)
{
	controlPoint->UpdateTriggers();

	for (auto triggerId : controlPoint->triggerSet)
	{
		AoiTrigger* pTrigger = mgr->triggerMap.GetData(triggerId);

		//画格子
		std::map<uint64_t, AoiGrid*>::iterator iter1 = mgr->gridMap.begin();
		for (; iter1 != mgr->gridMap.end(); ++iter1)
		{
			auto key = iter1->second->key;
			
			AoiGrid* pGrid = iter1->second;
			if (pTrigger->gridSet.find(iter1->first) != pTrigger->gridSet.end())
			{
				setlinecolor(0x800080);
				setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 3);
			}
			else
			{
				setlinecolor(BLACK);
				setlinestyle(PS_SOLID, 1);
			}
			rectangle(iter1->second->box.topLeft.x, iter1->second->box.topLeft.y, iter1->second->box.bottomRight.x, iter1->second->box.bottomRight.y);
		}


		// 画点
		std::map<uint64_t, AoiPoint*>::iterator iter = mgr->pointMap.objMap.begin();
		for (; iter != mgr->pointMap.objMap.end(); ++iter)
		{
			// control
			if (iter->second == controlPoint)
			{
				setlinestyle(PS_SOLID | PS_JOIN_BEVEL, 5);
				setlinecolor(RED);
				circle(iter->second->pos.x, iter->second->pos.y, pTrigger->exitDis);
				setlinecolor(GREEN);
				circle(iter->second->pos.x, iter->second->pos.y, pTrigger->enterDis);
				
			}
			else
			{
				setlinestyle(PS_SOLID, 1);
				if (pTrigger && pTrigger->pointSet.find(iter->second->uid) != pTrigger->pointSet.end())
				{
					setfillcolor(BLUE);
					fillcircle(iter->second->pos.x, iter->second->pos.y, 10);
				}
				else
				{
					setfillcolor(LIGHTGRAY);
					fillcircle(iter->second->pos.x, iter->second->pos.y, 3);
				}
			}
		}
	}

	

	RECT r = { 0, 0, 200, 20 };
	auto s = str_format("GridKey %d %d", controlPoint->pGrid->key.x, controlPoint->pGrid->key.y);
	drawtext(s.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void MessageOpe(AoiMgr* mgr)
{

	ExMessage m;		// 定义消息变量


		// 获取一条鼠标或按键消息
	m = getmessage(EM_MOUSE | EM_KEY);

	switch (m.message)
	{
	case WM_MOUSEMOVE:
		mousePos.x = m.x;
		mousePos.y = m.y;
		break;

	case WM_LBUTTONDOWN:
		// 如果点左键的同时按下了 Ctrl 键

		break;
	case WM_RBUTTONDOWN:

		break;
	case WM_KEYDOWN:
		switch (m.vkcode)
		{
		case 0x31:
			mgr->AddAoiPoint(new AoiPoint(mgr,1, Point2D(mousePos.x, mousePos.y)));
			break;
		case 0x32:
			controlPoint->SetPos(Point2D(mousePos.x, mousePos.y));
			break;
		case 0x33:
			controlPoint2->SetPos(Point2D(mousePos.x, mousePos.y));
			break;
		}
	}
}


void CollectTest(AoiMgr* mgr)
{
	controlPoint->UpdateTriggers();
	//画格子
	std::map<uint64_t, AoiGrid*>::iterator iter1 = mgr->gridMap.begin();
	for (; iter1 != mgr->gridMap.end(); ++iter1)
	{
		auto key = iter1->second->key;
		AoiGrid* pGrid = iter1->second;
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		rectangle(iter1->second->box.topLeft.x, iter1->second->box.topLeft.y, iter1->second->box.bottomRight.x, iter1->second->box.bottomRight.y);
	}


	auto lst = mgr->GetNearPoints(controlPoint->pos, 150, 1);

	// 画点
	std::map<uint64_t, AoiPoint*>::iterator iter = mgr->pointMap.objMap.begin();
	for (; iter != mgr->pointMap.objMap.end(); ++iter)
	{
		setfillcolor(LIGHTGRAY);
		fillcircle(iter->second->pos.x, iter->second->pos.y, 3);
	}

	float i = 10;
	for (auto point : lst)
	{
		setfillcolor(RED);
		fillcircle(point->pos.x, point->pos.y, i);
		i -= 0.5;
		i = max(i, 1);

	}
	
}

int main()
{	
	initgraph(mapSize.x+gridSize, mapSize.y+gridSize);	// 地图大小
	setbkcolor(WHITE);

	AoiMgr* mgr =  new AoiMgr(mapSize, gridSize);

	controlPoint = new AoiPoint(mgr,10, Point2D(mapSize.x/2, mapSize.y/2));
	mgr->AddAoiPoint(controlPoint);
	controlPoint2 = new AoiPoint(mgr,1, Point2D(mapSize.x / 2, mapSize.y / 2));
	mgr->AddAoiPoint(controlPoint2);

	for (uint32_t i = 0; i < gridSize * 10; i++)
	{
		mgr->AddAoiPoint(new AoiPoint(mgr,rand()%5, Point2D(rand()%int(mapSize.x),rand()%int(mapSize.y))));
	}

	auto id = mgr->CreateTrigger(1, 200, 20);

	controlPoint->BindTrigger(id);
	
	//	// 画圆，圆心(200, 200)，半径 100
	double lastUpdate = GetTickCount();
	while (true)
	{
		MessageOpe(mgr);
		if (GetTickCount() - lastUpdate > 30)
		{
			lastUpdate = GetTickCount();
			cleardevice();
			BeginBatchDraw();
			Draw(mgr);
			//CollectTest(mgr);
			EndBatchDraw();
		}
	}


	_getch();
	closegraph();			// 关闭绘图窗口
	std::cout << "sssss";
	return 0;
}


