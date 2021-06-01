// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

namespace Common
{

	template<typename T>
	typename TList<T>::CIterator TList<T>::Begin()
	{
		return CIterator(Head);
	}


	template<typename T>
	typename TList<T>::CConstIterator TList<T>::ConstBegin() const
	{
		return CConstIterator(Head);
	}

	template<typename T>
	typename TList<T>::CReverseIterator TList<T>::ReverseBegin()
	{
		return CReverseIterator(Tail);
	}

	template<typename T>
	typename TList<T>::CConstReverseIterator
		TList<T>::ConstReverseBegin() const
	{
		return CConstReverseIterator(Tail);
	}

	template<typename T>
	typename TList<T>::CSafeIterator TList<T>::SafeBegin()
	{
		return CSafeIterator(Head);
	}

	template<typename T>
	typename TList<T>::CSafeConstIterator
		TList<T>::SafeConstBegin() const
	{
		return CSafeConstIterator(Head);
	}

	template<typename T>
	typename TList<T>::CSafeReverseIterator
		TList<T>::SafeReverseBegin()
	{
		return CSafeReverseIterator(Tail);
	}

	template<typename T>
	typename TList<T>::CSafeConstReverseIterator
		TList<T>::SafeConstReverseBegin() const
	{
		return CSafeConstReverseIterator(Tail);
	}

	template<typename T>
	typename TList<T>::CIterator TList<T>::End()
	{
		return CIterator(Tail);
	}

	template<typename T>
	typename TList<T>::CConstIterator TList<T>::ConstEnd() const
	{
		return CConstIterator(Tail);
	}

	template<typename T>
	typename TList<T>::CReverseIterator TList<T>::ReverseEnd()
	{
		return CReverseIterator(Head);
	}

	template<typename T>
	typename TList<T>::CConstReverseIterator
		TList<T>::ConstReverseEnd() const
	{
		return CConstReverseIterator(Head);
	}

	template<typename T>
	typename TList<T>::CSafeIterator TList<T>::SafeEnd()
	{
		return CSafeIterator(Tail);
	}

	template<typename T>
	typename TList<T>::CSafeConstIterator
		TList<T>::SafeConstEnd() const
	{
		return CSafeConstIterator(Tail);
	}

	template<typename T>
	typename TList<T>::CSafeReverseIterator
		TList<T>::SafeReverseEnd()
	{
		return CSafeReverseIterator(Head);
	}

	template<typename T>
	typename TList<T>::CSafeConstReverseIterator
		TList<T>::SafeConstReverseEnd() const
	{
		return CSafeConstReverseIterator(Head);
	}

}