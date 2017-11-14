#pragma once
#include "Task.h"

void Task::matrixFilling()
{
	double hx;
	double localMatrix[4][4];
	vector<int> indexOfPoints;
	vector<bool> usedPoints;
	usedPoints.resize(grid.F.size(), false);
	int indexInMatrix[4];
	int cycleCounter = grid.greedX.size() - 1;
	for (int i = 0; i < cycleCounter; ++i)
	{
		indexOfPoints.resize(0);
		for (int k = 0; k < grid.X.size() && grid.X[k] <= grid.greedX[i + 1]; ++k)
		{
			if (grid.X[k] >= grid.greedX[i] && !usedPoints[k])
			{
				indexOfPoints.push_back(k);
				usedPoints[k] = true;
			}
		}
		if (indexOfPoints.size() == 0)
		{ 
			cout << "I can't find points on interval [" << grid.greedX[i]
				<< ", " << grid.greedX[i+1] << "] you must fix it, before do something." << endl;
			cin.get();
			system("exit");
		}
		hx = grid.greedX[i + 1] - grid.greedX[i];
		indexInMatrix[0] = 2 * i;
		indexInMatrix[1] = 2 * i + 1;
		indexInMatrix[2] = 2 * i + 2;
		indexInMatrix[3] = 2 * i + 3;
		for (int k = 0; k < indexOfPoints.size(); ++k)
		{
			for (int ii = 0; ii < 4; ++ii)
			{
				for (int jj = 0; jj < 4; ++jj)
				{
					a.setEl(indexInMatrix[ii], indexInMatrix[jj],
						basis.Psi(ii, grid.greedX[i], hx, grid.X[indexOfPoints[k]])*
						basis.Psi(jj, grid.greedX[i], hx, grid.X[indexOfPoints[k]]));
				}
				f[indexInMatrix[ii]] += basis.Psi(ii, grid.greedX[i], hx,
					grid.X[indexOfPoints[k]])*grid.F[indexOfPoints[k]];
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
	f.resize(2 * grid.greedX.size(), 0);
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
	ofstream outX("SplineX.txt");
	ofstream outF("SplineF.txt");
	double x, hx, summ;
	int k;
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
	outF << summ << endl;
	outF.close();
	outX.close();
}
