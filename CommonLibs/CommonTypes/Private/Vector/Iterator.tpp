// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

namespace Common
{

	template<typename T>
	typename TVector<T>::CIterator TVector<T>::Begin()
	{
		return CIterator(Buffer);
	}


	template<typename T>
	typename TVector<T>::CConstIterator TVector<T>::ConstBegin() const
	{
		return CConstIterator(Buffer);
	}

	template<typename T>
	typename TVector<T>::CReverseIterator TVector<T>::ReverseBegin()
	{
		return CReverseIterator(Buffer + Size);
	}

	template<typename T>
	typename TVector<T>::CConstReverseIterator
		TVector<T>::ConstReverseBegin() const
	{
		return CConstReverseIterator(Buffer + Size);
	}

	template<typename T>
	typename TVector<T>::CSafeIterator TVector<T>::SafeBegin()
	{
		return CSafeIterator(Buffer, this);
	}

	template<typename T>
	typename TVector<T>::CSafeConstIterator
		TVector<T>::SafeConstBegin() const
	{
		return CSafeConstIterator(Buffer, this);
	}

	template<typename T>
	typename TVector<T>::CSafeReverseIterator
		TVector<T>::SafeReverseBegin()
	{
		return CSafeReverseIterator(Buffer + Size, this);
	}

	template<typename T>
	typename TVector<T>::CSafeConstReverseIterator
		TVector<T>::SafeConstReverseBegin() const
	{
		return CSafeConstReverseIterator(Buffer + Size, this);
	}

	template<typename T>
	typename TVector<T>::CIterator TVector<T>::End()
	{
		return CIterator(Buffer + Size);
	}

	template<typename T>
	typename TVector<T>::CConstIterator TVector<T>::ConstEnd() const
	{
		return CConstIterator(Buffer + Size);
	}

	template<typename T>
	typename TVector<T>::CReverseIterator TVector<T>::ReverseEnd()
	{
		return CReverseIterator(Buffer);
	}

	template<typename T>
	typename TVector<T>::CConstReverseIterator
		TVector<T>::ConstReverseEnd() const
	{
		return CConstReverseIterator(Buffer);
	}

	template<typename T>
	typename TVector<T>::CSafeIterator TVector<T>::SafeEnd()
	{
		return CSafeIterator(Buffer + Size, this);
	}

	template<typename T>
	typename TVector<T>::CSafeConstIterator
		TVector<T>::SafeConstEnd() const
	{
		return CSafeConstIterator(Buffer + Size, this);
	}

	template<typename T>
	typename TVector<T>::CSafeReverseIterator
		TVector<T>::SafeReverseEnd()
	{
		return CSafeReverseIterator(Buffer, this);
	}

	template<typename T>
	typename TVector<T>::CSafeConstReverseIterator
		TVector<T>::SafeConstReverseEnd() const
	{
		return CSafeConstReverseIterator(Buffer, this);
	}

}