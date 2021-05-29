// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

#include "TypeOperations.h" // Swap
#include "AdvancedIteration.h"

namespace Common
{

	template <typename IteratorType, typename FunctionType>
	void BubbleSort(IteratorType Begin, IteratorType End,
		FunctionType Comparator);

	template<typename IteratorType, typename FunctionType>
	void SelectionSort(IteratorType Begin, IteratorType End,
		FunctionType Comparator);

	template<typename IteratorType, typename FunctionType>
	void QuickSort(IteratorType Begin, IteratorType End,
		FunctionType Comparator);

}

#include "Private/Sort.tpp"