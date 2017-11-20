#pragma once
#include "Task.h"

void Task::matrixFilling()
{
	int sizeX = grid.X.size() - 1, sizeY = grid.Y.size() - 1;
	double hx, hy;
	double localMatrix[4][4];
	vector<int> indexOfPoints;
	vector<bool> usedPoints;
	usedPoints.resize(grid.F.size(), false);
	int indexInMatrix[16];
	for (int j = 0; j < sizeY; ++j)
	{
		for (int i = 0; i < sizeX; ++i)
		{
			indexOfPoints.resize(0);
			for (int k = 0; k < grid.points.size() && grid.points[k].x <= grid.X[i + 1] && grid.points[k].y <= grid.Y[j + 1]; ++k)
			{
				if (grid.points[k].x >= grid.X[i] && grid.points[k].y >= grid.Y[i] && !usedPoints[k])
				{
					indexOfPoints.push_back(k);
					usedPoints[k] = true;
				}
			}
			if (indexOfPoints.size() == 0)
			{
				cout << "I can't find points on interval [" << grid.X[i]
					<< ", " << grid.X[i + 1] << "] * [" << grid.Y[i] << ", "
					<< grid.Y[i + 1] << "]\n you must fix it, before do something." << endl
					<< "Press any key, to exit." << endl;
				cin.get();
				system("exit");
			}
			hx = grid.X[i + 1] - grid.X[i];
			hy = grid.Y[i + 1] - grid.Y[i];
			int buf = 4 * grid.calculatePosistion(i, j);
			indexInMatrix[0] = buf;
			indexInMatrix[1] = buf + 1;
			indexInMatrix[2] = buf + 2;
			indexInMatrix[3] = buf + 3;
			int buf = 4 * grid.calculatePosistion(i, j + 1);
			indexInMatrix[4] = buf;
			indexInMatrix[5] = buf + 1;
			indexInMatrix[6] = buf + 2;
			indexInMatrix[7] = buf + 3;
			int buf = 4 * grid.calculatePosistion(i + 1, j);
			indexInMatrix[8] = buf;
			indexInMatrix[9] = buf + 1;
			indexInMatrix[10] = buf + 2;
			indexInMatrix[11] = buf + 3;
			int buf = 4 * grid.calculatePosistion(i + 1, j + 1);
			indexInMatrix[12] = buf;
			indexInMatrix[13] = buf + 1;
			indexInMatrix[14] = buf + 2;
			indexInMatrix[15] = buf + 3;
			for (int k = 0; k < indexOfPoints.size(); ++k)
			{
				for (int ii = 0; ii < 16; ++ii)
				{
					for (int jj = 0; jj < 16; ++jj)
					{
						a.setEl(indexInMatrix[ii], indexInMatrix[jj], basis.Psi(ii, grid.X[i], grid.Y[j],
							hx, hy, grid.points[indexOfPoints[k]].x, grid.points[indexOfPoints[k]].y)*
							basis.Psi(jj, grid.X[i], grid.Y[j],
								hx, hy, grid.points[indexOfPoints[k]].x, grid.points[indexOfPoints[k]].y));
					}
					f[indexInMatrix[ii]] += basis.Psi(ii, grid.X[i], grid.Y[j], hx, hy,
						grid.points[indexOfPoints[k]].x, grid.points[indexOfPoints[k]].y)*grid.F[indexOfPoints[k]];
				}
			}
		}
	}
}
void Task::make()
{
	vector<double> result;
	grid.input();
	list.fillingList(grid);
	a.profileDefining(list, grid);
	f.resize(4 * grid.X.size() * grid.Y.size(), 0);
	matrixFilling();
	//a.outMatrix();
	result = makeSLAU(a.di, a.al, a.au, a.ia, a.ja, f);
	/*ofstream out("Result.txt");
	for (int i = 0; i < result.size(); ++i)
	{
		out << result[i] << endl;
	}
	out.close();*/
	printSpline(0.05, result);
}

void Task::printSpline(double h, vector<double> result)
{
	int maxI = grid.X.size() - 1, maxJ = grid.Y.size() - 1;
	ofstream outX("SplineX.txt");
	ofstream outF("SplineF.txt");
	double x, hx, summ, y, hy;
	/*int k;
	for (k = 0; k < grid.greedX.size() - 1; ++k)
	{
		hx = grid.greedX[k + 1] - grid.greedX[k];
		x = grid.greedX[k];

		while (x < grid.greedX[k + 1] - 1e-10)
		{
			summ = 0;
			summ += result[  2 * k  ] * basis.Psi(0, grid.greedX[k], hx, x);
			summ += result[2 * k + 1] * basis.Psi(1, grid.greedX[k], hx, x);
			summ += result[2 * k + 2] * basis.Psi(2, grid.greedX[k], hx, x);
			summ += result[2 * k + 3] * basis.Psi(3, grid.greedX[k], hx, x);
			outX << x << endl;
			outF << summ << endl;
			x += h;
		}
	}
	--k;
	summ = 0;
	summ += result[2 * k] * basis.Psi(0, grid.greedX[k], hx, x);
	summ += result[2 * k + 1] * basis.Psi(1, grid.greedX[k], hx, x);
	summ += result[2 * k + 2] * basis.Psi(2, grid.greedX[k], hx, x);
	summ += result[2 * k + 3] * basis.Psi(3, grid.greedX[k], hx, x);
	outX << x << endl;
	outF << summ << endl;*/
	for (int i = 0; i < maxI; ++i)
	{
		for (int j = 0; j < maxJ; ++j)
		{

		}
	}
	outF.close();
	outX.close();
}
