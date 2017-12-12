#pragma once
#include <iostream>

using namespace std;

class Basis
{

public:
	double Psi(int i, double x, double y, double hx, double hy, double xi, double yi)
	{
		return Fi(Myu(i), xi, hx, x)*Fi(Nyu(i), yi, hy, y);
	}
	double Psi(int i, double xi, double hx, double x)//перегрузка для одномерной задачи
	{
		return Fi(i, xi, hx, x);
	}
	/*double Psi(Grid grid, int k, double xi, double yi)
	{
		int i = 0, j = 0;
		if (xi<grid.X[0] || xi>grid.X[grid.X.size() - 1] || yi<grid.Y[0] || yi>grid.Y[grid.Y.size() - 1])
		{
			cout << "Error in func Psi. Press any key to exit." << endl;
			cin.get();
			system("exit");
		}
		while (grid.X[i] <= xi)
			++i;
		while (grid.Y[j] <= yi)
			++j;
		return Psi(k, grid.X[i - 1], grid.Y[j - 1], grid.X[i] - grid.X[i - 1], grid.Y[j] - grid.Y[j - 1], xi, yi);

	}*/
	int Myu(int i)
	{
		return 2 * ((int)((i) / 4) % 2) + ((i) % 2);
	}
	int Nyu(int i)
	{
		return 2 * (int)((i) / 8) + ((int)((i) / 2) % 2);
	}

	double  secondComp(int i, double h)
	{
		return secondCompMatrix(Nyu(i), Myu(i), h);
	}

	double secondCompMatrix(int i, int j, double h)
	{
		int index = 4 * i + j;
		double var;
		switch (index)
		{
		case 0:
			var = 36;
			break;
		case 1:
			var = 3 * h;
			break;
		case 2:
			var = -36;
			break;
		case 3:
			var = 3 * h;
			break;
		case 4:
			var = 3 * h;
			break;
		case 5:
			var = 4 * h *h;
			break;
		case 6:
			var = -3 * h;
			break;
		case 7:
			var = -h * h;
			break;
		case 8:
			var = -36;
			break;
		case 9:
			var = -3 * h;
			break;
		case 10:
			var = 36;
			break;
		case 11:
			var = -3 * h;
			break;
		case 12:
			var = 3 * h;
			break;
		case 13:
			var = -h * h;
			break;
		case 14:
			var = -3 * h;
			break;
		case 15:
			var = 4 * h * h;
			break;
		default:
			cout << "TY CHE DAUN?" << endl;
			cin.get();
			break;
			return var/(30 * h);
		}
	}

private:
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