// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

namespace Common
{

	template<typename T>
	inline T* TVector<T>::Allocate(const size_t AllocSize)
	{
		try
		{
			return reinterpret_cast<T*>(::operator new (sizeof(T) * AllocSize));
		}
		catch (...)		// use own exception system
		{
			throw CBadAlloc("Vector: Failed to allocate", sizeof(T) * AllocSize);
		}
	}


	template<typename T>
	inline void TVector<T>::Deallocate(T*& OutBuffer) noexcept
	{
		::operator delete[](OutBuffer);
		OutBuffer = nullptr;
	}


	template<typename T>
	inline void TVector<T>::Construct(const size_t Index, T* const OutBuffer,
		const T& Value)
	{
		new (OutBuffer + Index) T(Value);
	}


	template<typename T>
	inline void TVector<T>::Destruct(const size_t Index,
		T* const OutBuffer) noexcept
	{
		OutBuffer[Index].~T();
	}


	template<typename T>
	inline void TVector<T>::DestructRange(const size_t From, const size_t To,
		T* const OutBuffer) noexcept
	{
		for (size_t i = From; i < To; ++i) {
			Destruct(i, OutBuffer);
		}
	}


	template<typename T>
	inline void TVector<T>::DestructAll(const size_t Size,
		T* const OutBuffer) noexcept
	{
		DestructRange(0, Size, OutBuffer);
	}


	template<typename T>
	void TVector<T>::SafeMoveBlock(const size_t Size,
		T* const FromBuffer, T* const ToBuffer)
	{
		size_t i = 0;
		try
		{
			while (i < Size)
			{
				Construct(i, ToBuffer, Move(FromBuffer[i]));
				Destruct(i, FromBuffer);
				++i;
			}
		}
		catch (...)
		{
			DestructRange(0, i, ToBuffer);
			DestructRange(i, Size, FromBuffer);
		}
	}


	template<typename T>
	void TVector<T>::SafeMoveBlockReverse(const size_t Size,
		T* const FromBuffer, T* const ToBuffer)
	{
		size_t i = Size;
		try
		{
			while (i >= 1)	// i is unsigned; cannot compare to 0 there
			{
				--i;
				Construct(i, ToBuffer, Move(FromBuffer[i]));
				Destruct(i, FromBuffer);
			}
		}
		catch (...)
		{
			DestructRange(i + 1, Size, ToBuffer);
			DestructRange(0, i + 1, FromBuffer);
		}
	}


	template<typename T>
	void TVector<T>::Reconstruct(size_t CopySize, size_t AllocSize,
		T*& OutBuffer, size_t& OutCopySize, size_t& OutAllocSize)
	{
		T* TempBuffer = Allocate(AllocSize);
		try
		{
			SafeMoveBlock(CopySize, OutBuffer, TempBuffer);
		}
		catch (...)
		{
			Deallocate(TempBuffer);
			OutCopySize = 0;
			throw;
		}

		::Swap(TempBuffer, OutBuffer);
		OutCopySize = CopySize;
		OutAllocSize = AllocSize;

		Deallocate(TempBuffer);
	}


	template<typename T>
	template<typename IteratorType>
	void TVector<T>::SafeBulkConstruct(const size_t StartPosition,
		const IteratorType Begin, const IteratorType End, T* const OutBuffer)
	{
		size_t i = StartPosition;
		try
		{
			for (auto it = Begin; it != End; ++it)
			{
				Construct(i, OutBuffer, *it);
				++i;
			}
		}
		catch (...)
		{
			DestructRange(StartPosition, i, OutBuffer);
			throw;
		}
	}


	template<typename T>
	void TVector<T>::SafeFillConstruct(const size_t StartPosition,
		const size_t EndPosition, T* const OutBuffer, const T& Value)
	{
		size_t i = StartPosition;
		try
		{
			while (i < EndPosition)
			{
				Construct(i, OutBuffer, Value);
				++i;
			}
		}
		catch (...)
		{
			DestructRange(StartPosition, i, OutBuffer);
			throw;
		}
	}

}