// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

#include <new>

#include "./../CommonTypes/Exception.h"
#include "TypeOperations.h"	// Move, Swap

namespace Common
{
	template <typename T>
	inline void Allocate(size_t NewSize, T*& OutBuffer);

	template <typename T>
	inline void Deallocate(T*& OutBuffer) noexcept;

	template <typename T>
	inline void Construct(size_t Index, T* OutBuffer, const T& Value);

	template <typename T>
	inline void Destruct(size_t Index, T* OutBuffer) noexcept;


	template <typename T>
	inline void DestructRange(size_t From, size_t To,
		T* OutBuffer) noexcept;	// [From; To)

	template <typename T>
	inline void DestructAll(size_t Size, T* OutBuffer) noexcept;


	template <typename T>
	void SafeMoveBlock(size_t Size, T* FromBuffer, T* ToBuffer);

	template <typename T>
	void SafeMoveBlockReverse(size_t Size, T* FromBuffer, T* ToBuffer);

	template <typename T>
	void Reconstruct(size_t CopySize, size_t NewCapacity,
		T*& OutBuffer, size_t& OutCapacity, size_t& OutSize);

	template <typename IteratorType, typename T>
	void SafeBulkConstruct(size_t StartPosition, IteratorType From,
		IteratorType To, T* OutBuffer);

	template <typename T>
	void SafeFillConstruct(size_t StartPosition, size_t EndPosition,
		T* OutBuffer, const T& Value);

}

#include "Private/BlockAllocation.tpp"