#pragma once
#include <iostream>

using namespace std;

class Basis
{

public:
	double Psi(int i, double x, double y, double hx, double hy, double xi, double yi)
	{
		return Fi(Myu(i), x, hx, xi)*Fi(Nyu(i), y, hy, yi);
	}
	double Psi(int i, double xi, double hx, double x)//перегрузка для одномерной задачи
	{

		return Fi(i, xi, hx, x);

	}
private:
	int Myu(int i)
	{
		return 2 * ((int)((i - 1) / 4) % 2) + ((i - 1) % 2) + 1;
	}
	int Nyu(int i)
	{
		return 2 * (int)((i - 1) / 8) + ((int)((i - 1) / 2) % 2) + 1;
	}
	double Ksi(double xi, double h, double x)
	{
		return (x - xi) / h;
	}
	double Fi(int i, double xi, double h, double x)
	{
		double ksi = Ksi(xi, h, x);
		switch (i)
		{
		case 0:
			return 1 - 3 * ksi*ksi + 2 * ksi*ksi*ksi;
		case 1:
			return ksi - 2 * ksi*ksi + ksi*ksi*ksi;
		case 2:
			return 3 * ksi*ksi - 2 * ksi*ksi*ksi;
		case 3:
			return -ksi*ksi + ksi*ksi*ksi;
		default:
			cout << "TY CHE DURAK?" << endl;
			cin.get();
			break;
		}
	}
};