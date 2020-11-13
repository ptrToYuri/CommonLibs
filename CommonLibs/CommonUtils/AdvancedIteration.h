#pragma once

namespace Common 
{

	template <typename IteratorType>
	unsigned int GetIteratorDistance(IteratorType Begin, IteratorType End)
	{
		unsigned int Distance = 0;
		while (Begin != End)
		{
			++Begin;
			++Distance;
		}
		return Distance;
	}

}
