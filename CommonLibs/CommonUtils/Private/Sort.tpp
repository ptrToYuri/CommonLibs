// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

namespace Common
{

	template <typename IteratorType, typename FunctionType>
	void BubbleSort(IteratorType Begin, const IteratorType End,
		FunctionType Comparator)
	{
		for (auto it1 = Begin; it1 != End; ++it1)
		{
			for (auto it2 = Begin; it2 != it1; ++it2)
			{
				if (Comparator(*it1, *it2))
				{
					Swap(*it1, *it2);
				}
			}
		}
	}


	template<typename IteratorType, typename FunctionType>
	void SelectionSort(IteratorType Begin, IteratorType End,
		FunctionType Comparator)
	{
		IteratorType Min;
		IteratorType it2;
		for (auto it1 = Begin; it1 != End; ++it1)
		{
			Min = it1;
			for (it2 = it1 + 1; it2 != End; ++it2)
			{
				if (Comparator(*it2, *Min))
				{
					Swap(*it2, *Min);
				}
			}
		}
	}


	// hide this later
	template <typename IteratorType, typename FunctionType>
	IteratorType Partition(IteratorType Begin, IteratorType End, FunctionType Compare)
	{
		IteratorType Pivot = End - 1;
		IteratorType it1 = Begin;
		for (auto it2 = Begin; it2 != Pivot; ++it2) {
			if (Compare(*it2, *Pivot)) {
				Swap(*it1, *it2);
				++it1;
			}
		}
		Swap(*it1, *Pivot);
		return it1;
	}

	template <typename IteratorType, typename FunctionType>
	void QuickSort(IteratorType Begin, IteratorType End, FunctionType Compare)
	{
		if (GetIteratorDistance(Begin, End) > 1) {
			IteratorType Bound = Partition(Begin, End, Compare);
			QuickSort(Begin, Bound, Compare);
			QuickSort(Bound + 1, End, Compare);
		}
	}


}