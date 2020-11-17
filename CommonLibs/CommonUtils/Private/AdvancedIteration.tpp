// Yuri Zamyatin, 2020. This file is part of CommonLibs

namespace Common
{

	template <typename IteratorType>
	size_t GetIteratorDistance(IteratorType Begin, IteratorType End)
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