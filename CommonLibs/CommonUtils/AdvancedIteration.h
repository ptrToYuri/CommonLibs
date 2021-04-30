// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

namespace Common 
{

	/**
	 * @brief Counts elements between two iterators. Range: [Begin: End)
	 * @tparam IteratorType Iterator with implemented ++, != and *
	 * @param Begin Iterator referring to the first element
	 * @param End Iterator referring to the element after last one
	 * @return Distance between iterators
	 * @note Begin must not be greater than end (negative results are
	 *		 not supported)
	*/
	template <typename IteratorType>
	size_t GetIteratorDistance(IteratorType Begin, IteratorType End);

}

#include "Private/AdvancedIteration.tpp"