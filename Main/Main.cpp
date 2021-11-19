
#include <iostream>
#include <windows.h>
#include <stdlib.h>
#include <chrono>
#include <map>
#include "AoiMgr.h"
#include "CustomMap.h"
using namespace std;
#include <bitset>
#include "CustomAlgorithm.h"


int main()
{
	using namespace std::chrono;


//     uint64_t testCount = 10 * 10000;
//     high_resolution_clock::time_point t1 = high_resolution_clock::now();
// 
//     high_resolution_clock::time_point t2 = high_resolution_clock::now();
// 
//     duration<double, std::milli> time_span = t2 - t1;
// 
//     cout << "创建耗时："<<time_span.count() << endl;
// 
// 
// 
//     uint32_t gridSize = 5;
//     for (int i = 0; i < 20; ++i)
//         cout << i / gridSize * gridSize << endl;
// 
//    
//     while (true)
//     {
//         
//     }

	AoiMgr* mgr = new AoiMgr(Point2D(512, 512), 20);

	auto point1 = new AoiPoint(1, Point2D(1, 2));
	mgr->AddAoiPoint(*point1);
	mgr->AddAoiPoint(*new AoiPoint(1<<1, Point2D(17, 8)));
	mgr->AddAoiPoint(*new AoiPoint(1<<2, Point2D(30, 25)));
	mgr->AddAoiPoint(*new AoiPoint(1 << 2, Point2D(31, 24)));
	mgr->AddAoiPoint(*new AoiPoint(1<<3, Point2D(100, 215)));
	mgr->AddAoiPoint(*new AoiPoint(uint64_t(1)<<63, Point2D(100, 215)));
	mgr->AddAoiPoint(*new AoiPoint(3, Point2D(500, 490)));
	mgr->AddAoiPoint(*new AoiPoint(5, Point2D(501, 491)));


	//point1->SetPos(Point2D(31,24));
	

	for (map<uint64_t,AoiGrid*>::iterator iter = mgr->gridMap.begin(); iter != mgr->gridMap.end(); ++iter)
	{		

		iter->second->Debug();
	}

	SAFE_DELETE(mgr);
}
