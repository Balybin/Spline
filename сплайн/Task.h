#pragma once
#include "Matrix.h"
#include "Grid.h"
#include "Basis.h"
#include "ListOfAdjacency.h"
#include <math.h>
#include <iomanip>

class Task
{
public:
	Matrix a;
	vector<double> f;
	ListOfAdjacency list;
	Grid grid;
	Basis basis;
	void matrixFilling();
	void make();
	void printSpline(double h, vector<double> result);
};

extern vector<double> makeSLAU(vector<double> &_di, vector<double> &_al, vector<double> &_au, vector<int> &_ia, vector<int> &_ja, vector<double> &_F);