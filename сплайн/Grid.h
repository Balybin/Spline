#pragma once
#include <fstream>
#include <vector>

using namespace std;

class Grid
{
public:
	vector<double> X, F;//, Y; ���� ��� ����������.
	vector<double> greedX;
	void input();
	
};