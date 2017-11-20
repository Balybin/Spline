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

int Grid::calculatePosistion(int i, int j)
{
	if (i >= X.size() || j >= Y.size() || i < 0 || j < 0)
		return -1;
	int k = j * X.size() + i;
	if (k >= 0)
		return k;
	else
		return -1;
}

void Grid::input()
{
	ifstream file("InputData.txt");
	int Qx, Qy, Qf;
	double buf;
	file >> Qx;
	for (int i = 0; i < Qx; ++i)
	{
		file >> buf;
		X.push_back(buf);
	}
	file >> Qy;
	for (int i = 0; i < Qy; ++i)
	{
		file >> buf;
		Y.push_back(buf);
	}
	Qf = Qy*Qx;
	for (int i = 0; i < Qf; ++i)
	{
		file >> buf;
		F.push_back(buf);
	}
	for (int i = 0; i < Qf; ++i)
	{
		Point point;
		file >> point.x >> point.y;
		points.push_back(point);
	}
	file.close();
}