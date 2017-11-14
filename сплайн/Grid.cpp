#pragma once
#include "Grid.h"
//(количество точек f)
//N
//f1, ... fn
//x1, ... xn
//(количество узлов сетки сплайна)
//M
//(узлы сплайна)
//x1, ... xm
void Grid::input()
{
	ifstream file("InputData.txt");
	int Qx, Qy, Qf;
	double buf;
	file >> Qf;// >> Qy;
	for (int i = 0; i < Qf; ++i)
	{
		file >> buf;
		F.push_back(buf);
	}
	for (int i = 0; i < Qf; ++i)
	{
		file >> buf;
		X.push_back(buf);
	}
	//for (int i = 0; i < Qy; ++i)
	//{
	//	file >> buf;
	//	Y.push_back(buf);
	//}
	file >> Qx;
	for (int i = 0; i < Qx; ++i)
	{
		file >> buf;
		greedX.push_back(buf);
	}
	file.close();
}