#pragma once
#include "Task.h"

void Task::matrixFilling()
{
	ofstream file("right.txt");
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
			for (int k = 0; k < grid.points.size(); ++k)
			{
				if (!usedPoints[k] &&
					grid.points[k].x >= grid.X[i] && grid.points[k].x <= grid.X[i + 1] &&
					grid.points[k].y >= grid.Y[j] && grid.points[k].y <= grid.Y[j + 1])
				{
					indexOfPoints.push_back(k);
					usedPoints[k] = true;
				}
			}
			if (indexOfPoints.size() == 0)
			{
				cout << "I can't find points on interval [" << grid.X[i]
					<< ", " << grid.X[i + 1] << "] * [" << grid.Y[j] << ", "
					<< grid.Y[j + 1] << "]\n you must fix it, before do something." << endl
					<< "Press any key, to exit." << endl;
				cin.get();
				system("exit");
			}
			hx = grid.X[i + 1] - grid.X[i];
			hy = grid.Y[j + 1] - grid.Y[j];
			int buf = 4 * grid.calculatePosistion(i, j);
			indexInMatrix[0] = buf;
			indexInMatrix[1] = buf + 1;
			indexInMatrix[2] = buf + 2;
			indexInMatrix[3] = buf + 3;
			buf = 4 * grid.calculatePosistion(i + 1, j);
			indexInMatrix[4] = buf;
			indexInMatrix[5] = buf + 1;
			indexInMatrix[6] = buf + 2;
			indexInMatrix[7] = buf + 3;
			buf = 4 * grid.calculatePosistion(i, j + 1);
			indexInMatrix[8] = buf;
			indexInMatrix[9] = buf + 1;
			indexInMatrix[10] = buf + 2;
			indexInMatrix[11] = buf + 3;
			buf = 4 * grid.calculatePosistion(i + 1, j + 1);
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
						a.setEl(indexInMatrix[ii], indexInMatrix[jj], grid.omega[k]*basis.Psi(ii, grid.X[i], grid.Y[j],
							hx, hy, grid.points[indexOfPoints[k]].x, grid.points[indexOfPoints[k]].y)*
							basis.Psi(jj, grid.X[i], grid.Y[j], hx, hy, 
								grid.points[indexOfPoints[k]].x, grid.points[indexOfPoints[k]].y)
							+ basis.secondComp(0, ii, jj, hx, hy)
							+ basis.thirdComp(1e7, ii, jj, hx, hy));
					}
					f[indexInMatrix[ii]] += basis.Psi(ii, grid.X[i], grid.Y[j], hx, hy,
						grid.points[indexOfPoints[k]].x, grid.points[indexOfPoints[k]].y)*(grid.F[indexOfPoints[k]]+grid.error[k]);
					file << basis.Psi(ii, grid.X[i], grid.Y[j], hx, hy,
						grid.points[indexOfPoints[k]].x, grid.points[indexOfPoints[k]].y)*grid.F[indexOfPoints[k]] << "\t";
				}
				file << endl;
			}
		}
	}
	file.close();
}
void Task::make()
{
	
	vector<double> result;
	grid.input();
	a.profileDefining(list, grid);
	f.resize(4 * grid.X.size() * grid.Y.size(), 0);
	double meanDev = meanDeviation();
	bool flag = true;
	while (flag)
	{
		flag = false;
		matrixFilling();
		result = makeSLAU(a.di, a.al, a.au, a.ia, a.ja, f);
		for (int i = 0; i < grid.points.size(); ++i)
		{
			if (grid.error[i] > 3 * valueInPoint(grid.points[i].x, grid.points[i].y, result))
			{
				flag = true;
				grid.omega[i] /= 2;
			}
		}
		a.nullMatrix();
	}
	a.outMatrix();
	ofstream out("Result.txt");
	for (int i = 0; i < result.size(); ++i)
	{
		out << result[i] << endl;
	}
	out.close();
	printSpline(0.05, 0.05, result);
}
double Task::meanDeviation()
{
	double sum;
	for (int i = 0; i < grid.error.size(); ++i)
		sum += abs(grid.error[i]);
	sum /= grid.error.size();
	return sum;
}

double Task::valueInPoint(double x, double y, vector<double> result)
{
	int maxI = grid.X.size() - 1, maxJ = grid.Y.size() - 1;
	int i = 0, j = 0;
	double summ=0;
	while (grid.X[i] < x && i < maxI - 1)
		++i;
	if (i > 0) --i;
	double hx = grid.X[i + 1] - grid.X[i];
	if (y > grid.Y[maxJ]) y = grid.Y[maxJ];
	while (grid.Y[j] < y && j < maxJ - 1)
		++j;
	if (j > 0) --j;
	double hy = grid.Y[j + 1] - grid.Y[j];
	int NumOfEl = 4 * grid.calculatePosistion(i, j);
	for (int k = 0; k < 16; ++k)
	{
		summ += result[NumOfEl + k] * basis.Psi(k, grid.X[i], grid.Y[j], hx, hy, x, y);
	}
	return summ;
}

void Task::printSpline(double hx, double hy, vector<double> result)
{
	int maxI = grid.X.size() - 1, maxJ = grid.Y.size() - 1;
	int kMax = grid.points.size();
	ofstream outX("SplineX.txt");
	ofstream layer("layer.txt");
	ofstream outF("SplineF.txt");
	ofstream outY("SplineY.txt");
	vector<bool> isUsed;
	vector<int> indexesOfPoints;
	isUsed.resize(kMax, false);
	double summ,hXforBasis,hYforBasis;
	for (double y = grid.Y[0]; y < grid.Y[maxJ] + 1e-10; y += hy)
	{
		for (double x = grid.X[0]; x < grid.X[maxI] + 1e-10; x += hx)
		{
			int i = 0, j = 0;
			summ = 0;
			while (grid.X[i] < x && i < maxI - 1)
				++i;
			if (i > 0) --i;
			hXforBasis = grid.X[i + 1] - grid.X[i];
			if (y > grid.Y[maxJ]) y = grid.Y[maxJ];
			while (grid.Y[j] < y && j < maxJ - 1)
				++j;
			if (j > 0) --j;
			hYforBasis = grid.Y[j + 1] - grid.Y[j];
			int NumOfEl =4 * grid.calculatePosistion(i, j);
			for (int k = 0; k < 16; ++k)
			{
				summ += result[NumOfEl + k] * basis.Psi(k, grid.X[i], grid.Y[j], hXforBasis, hYforBasis, x, y);
			}
			/*if (i == 0)
			{
				layer << summ << endl;
			}*/
			if(y == grid.Y[0]) outX << x << endl;
			outF << summ << endl;
		}
		outY << y << endl;
	}
	outF.close();
	outX.close();
	outY.close();
}
