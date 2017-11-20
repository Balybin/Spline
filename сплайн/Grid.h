#pragma once
#include <fstream>
#include <vector>

using namespace std;

struct Point
{
	double x, y;
};

class Grid
{
public:
	vector<Point> points;
	vector<double> X, Y, F;
	void input();
	int calculatePosistion(int i, int j);
};