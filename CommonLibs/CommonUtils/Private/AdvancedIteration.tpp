// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

namespace Common
{

	template <typename IteratorType>
	size_t GetIteratorDistance(IteratorType Begin,
		const IteratorType End)
	{
		size_t Distance = 0;
		while (Begin != End)
		{
			++Begin;
			++Distance;
		}
		return Distance;
	}

}