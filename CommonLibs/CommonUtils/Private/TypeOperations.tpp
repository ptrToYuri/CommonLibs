// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

namespace Common
{

	template<typename T>
	typename RemoveReference<T>::Type&& Move(T&& Value)
	{
		return ((typename RemoveReference<T>::Type &&)Value);
	}


	template<typename T>
	void Swap(T& First, T& Second) {
		T Temp(Move(First));
		First = Move(Second);
		Second = Move(Temp);
	}

}