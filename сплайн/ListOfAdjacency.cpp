#pragma once
#include "ListOfAdjacency.h"
void BubbleSort(vector<int> &A)
{
	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < A.size() - i - 1; j++)
		{
			if (A[j] > A[j + 1])
			{
				int temp = A[j];
				A[j] = A[j + 1];
				A[j + 1] = temp;
			}
		}
	}
}
void ListOfAdjacency::fillingList(Grid grid)
{
	list.resize(2 * grid.greedX.size());
	for (int i = 0; i < grid.greedX.size(); ++i)
	{
		addToList(i, i);
		if (i - 1 >= 0) addToList(i, i - 1);
		//list[i].push_back(i - 1);
	}
	for (int i = 0; i < 2*grid.greedX.size(); ++i)
	{
		BubbleSort(list[i]);
	}
}
int ListOfAdjacency::addToList(int i, int j)
{
	if (list.size() == 0) return -1;
	if (i == j)
		list[2 * i + 1].push_back(2 * j);
	else if (j < i)
	{
		list[2 * i].push_back(2 * j);
		list[2 * i].push_back(2 * j + 1);
		list[2 * i + 1].push_back(2 * j);
		list[2 * i + 1].push_back(2 * j + 1);
	}
	return 0;
}