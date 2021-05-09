// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

namespace Common
{

	template <typename T>
	struct RemoveReference { typedef T Type; };

	template <typename T>
	struct RemoveReference<T&> { typedef T Type; };

	template <typename T>
	struct RemoveReference<T&&> { typedef T Type; };


	template<typename T>
	typename RemoveReference<T>::Type&& Move(T&& Value);


	template<typename T>
	void Swap(T& First, T& Second);

}

#include "Private/TypeOperations.tpp"