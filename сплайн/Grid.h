#pragma once
#include <fstream>
#include <vector>

using namespace std;

class Grid
{
public:
	vector<double> X, F;//, Y; пока для одномерной.
	vector<double> greedX;
	void input();
	
};