// Yuri Zamyatin, 2020-2021. This file is part of LibTests

#include "../Sort.h"

using namespace Common;

void BubbleSortTest()
{
	TVector<int> a = { 5,6,4,3,2,1,10,7 };
	QuickSort(a.Begin(), a.end(),
		[](int First, int Second)
		{ return First < Second; });
	/*
	for (auto el : a)
	{
		std::cout << el << " ";
	}
	*/

	//todo: tests
}