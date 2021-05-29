// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

namespace Common
{

	template<typename T>
	TVector<T>::TVector(const EReservedCapacityRule CapacityRule) noexcept
		: CapacityRule(CapacityRule) {};


	template<typename T>
	TVector<T>::TVector(const size_t Size, const T& DefaultValue,
		const EReservedCapacityRule CapacityRule)
		: Capacity(Size), CapacityRule(CapacityRule)
	{
		if (!Capacity)
		{
			return;
		}

		Allocate(Capacity, Buffer);
		SafeFillConstruct(0, Size, Buffer, DefaultValue);
		this->Size = Size;
	}


	template<typename T>
	TVector<T>::TVector(const size_t Size, const T* const Array,
		const EReservedCapacityRule CapacityRule)
		: Capacity(Size), CapacityRule(CapacityRule)
	{
		if (!Capacity)
		{
			return;
		}

		Allocate(Capacity, Buffer);
		SafeBulkConstruct(0, Array, Array + Size, Buffer);
		this->Size = Size;
	}


	template<typename T>
	TVector<T>::TVector(const std::initializer_list<T>& ValuesList)
		: Capacity(ValuesList.size())
	{
		if (!Capacity)
		{
			return;
		}

		Allocate(Capacity, Buffer);
		SafeBulkConstruct(0, ValuesList.begin(), ValuesList.end(), Buffer);
		Size = Capacity;
	}


	template <typename T>
	template <typename IteratorType>
	TVector<T>::TVector(const IteratorType Begin, const IteratorType End,
		EReservedCapacityRule CapacityRule,
		// disable this constructor when it "wins" the first one
		typename std::enable_if<!std::is_integral<
		IteratorType>::value>::type*)

		: Capacity(GetIteratorDistance(Begin, End)),
		CapacityRule(CapacityRule)
	{
		if (!Capacity)
		{
			return;
		}

		Allocate(Capacity, Buffer);
		SafeBulkConstruct(0, Begin, End, Buffer);
		Size = Capacity;
	}


	template<typename T>
	TVector<T>::TVector(const TVector<T>& Other)
		: Capacity(Other.Capacity),
		CapacityRule(Other.CapacityRule)
	{
		if (!Capacity)
		{
			return;
		}

		Allocate(Capacity, Buffer);
		SafeBulkConstruct(0, Other.Buffer, Other.Buffer + Other.Size, Buffer);
		Size = Other.Size;
	}


	template<typename T>
	TVector<T>::TVector(TVector<T>&& Other) noexcept
	{
		Swap(Other);
	}


	template<typename T>
	TVector<T>::~TVector()
	{
		DestructAll(Size, Buffer);
		Deallocate(Buffer);
	}



	template<typename T>
	template<typename IteratorType>
	void TVector<T>::Assign(const IteratorType Begin,
		const IteratorType End, const EShrinkBehavior ShrinkBehavior)
	{
		size_t NewSize = GetIteratorDistance(Begin, End);
		size_t NewCapacity = CalcExtendedCapacity(NewSize);

		T* TempBuffer;
		Allocate(NewCapacity, TempBuffer);
		try
		{
			SafeBulkConstruct(0, Begin, End, TempBuffer);
		}
		catch (...)
		{
			Deallocate(TempBuffer);
			throw;
		}

		::Swap(TempBuffer, Buffer);
		::Swap(Size, NewSize);
		Capacity = NewCapacity;

		DestructAll(NewSize, TempBuffer);
		Deallocate(TempBuffer);
	}


	template <typename T>
	TVector<T>& TVector<T>::operator = (const
		std::initializer_list<T>& ValuesList)
	{
		Assign(ValuesList.begin(), ValuesList.end());
		return *this;
	}


	template<typename T>
	TVector<T>& TVector<T>::operator = (const TVector<T>& Other)
	{
		size_t NewSize = Other.Size;

		T* TempBuffer;
		Allocate(Other.Capacity, TempBuffer);
		try
		{
			SafeBulkConstruct(0, Other.Buffer,
				Other.Buffer + Other.Size, TempBuffer);
		}
		catch (...)
		{
			Deallocate(TempBuffer);
			throw;
		}

		::Swap(TempBuffer, Buffer);
		::Swap(Size, NewSize);
		Capacity = Other.Capacity;

		DestructAll(NewSize, TempBuffer);
		Deallocate(TempBuffer);

		return *this;
	}


	template<typename T>
	TVector<T>& TVector<T>::operator = (TVector<T>&& Other) noexcept
	{
		Swap(Other);
		return *this;
	}



	template<typename T>
	T& TVector<T>::operator [](const size_t Index)
	{
		ASSERT(Index < Size, "Out of range: [] vector");
		return Buffer[Index];
	}


	template<typename T>
	T& TVector<T>::SafeAt(const size_t Index)
	{
		if (Index >= Size)
		{
			throw COutOfRange("Out of range: [] vector",
				static_cast<int>(Index), TPair<size_t, size_t>(0, Size));
		}

		return Buffer[Index];
	}


	template<typename T>
	T& TVector<T>::AutoAt(const size_t Index, const T& DefaultValue)
	{
		if (Index >= Size)
		{
			Resize(Index + 1, DefaultValue);
		}

		return Buffer[Index];
	}


	template<typename T>
	T* TVector<T>::RawData() noexcept
	{
		return Buffer;
	}



	template<typename T>
	bool TVector<T>::operator == (const TVector<T>& Other) const noexcept
	{
		if (Size != Other.Size)
		{
			return false;
		}

		for (size_t i = 0; i < Size; ++i)
		{
			if (!(Buffer[i] == Other.Buffer[i]))
			{
				return false;
			}
		}
		return true;
	}


	template<typename T>
	bool TVector<T>::operator != (const TVector<T>& Other) const noexcept
	{
		return !operator==(Other);
	}


	template<typename T>
	TVector<T>& TVector<T>::operator += (TVector<T>& Other)
	{
		Push(Other.Buffer, Other.Buffer + Other.Size);
		return *this;
	}


	template<typename T>
	TVector<T> TVector<T>::operator + (const TVector<T>& Other) const
	{
		size_t NewSize = Size + Other.Size;
		TVector<T> NewVector;
		Allocate(NewSize, NewVector.Buffer);
		NewVector.Capacity = NewSize;

		SafeBulkConstruct(0, Buffer, Buffer + Size, NewVector.Buffer);
		NewVector.Size = Size;
		SafeBulkConstruct(Size, Other.Buffer, Other.Buffer + Other.Size, 
			NewVector.Buffer);
		NewVector.Size = NewSize;

		return NewVector;
	}


	template <typename T>
	void TVector<T>::Push(const T& Value)
	{
		if (Size + 1 > Capacity)
		{
			Reconstruct(Size, CalcExtendedCapacity(Size + 1),
				Buffer, Size, Capacity);
		}
		Construct(Size, Buffer, Value);
		++Size;
	}


	template <typename T>
	template <typename IteratorType>
	void TVector<T>::Push(const IteratorType Begin, const IteratorType End)
	{
		size_t Distance = GetIteratorDistance(Begin, End);

		if (Size + Distance > Capacity)
		{
			Reconstruct(Size, CalcExtendedCapacity(Size + Distance),
				Buffer, Size, Capacity);
		}
		SafeBulkConstruct(Size, Begin, End, Buffer);
		Size += Distance;
	}



	template <typename T>
	void TVector<T>::Insert(const size_t Position, const T& Value)
	{
		Insert(Position, &Value, &Value + 1);
	}


	template <typename T>
	void TVector<T>::SafeInsert(const size_t Position, const T& Value)
	{
		SafeInsert(Position, &Value, &Value + 1);
	}


	template <typename T>
	void TVector<T>::AutoInsert(const size_t Position, const T& Value,
		const T& DefaultValue)
	{
		AutoInsert(Position, &Value, &Value + 1, DefaultValue);
	}


	template <typename T>
	template <typename IteratorType>
	void TVector<T>::Insert(const size_t Position, const IteratorType Begin,
		const IteratorType End)
	{
		ASSERT(Position <= Size, "Out of range: vector insert");

		size_t Distance = GetIteratorDistance(Begin, End);

		// Step 1: reconstruct up to insert position
		// Note: clears vector if Move construction fails
		if (Size + Distance > Capacity)
		{
			Reconstruct(Size, CalcExtendedCapacity(Size + Distance),
				Buffer, Size, Capacity);
		}

		// Step 2: Move old elements out of insertion position
		// Note: clears vector if Move construction fails
		try
		{
			SafeMoveBlockReverse(Size - Position, Buffer + Position,
				Buffer + Position + Distance);
		}
		catch (...)
		{
			DestructRange(0, Position, Buffer);	// SafeMoveBlock destructed the rest
			Size = 0;
			AutoShrinkIfNeeded(EShrinkBehavior::Allow);
			throw;
		}

		// Step 3: Copy elements to insert
		// Note: If copy construction fails, tries to revert vector 
		// to a previous state. In case of Move faulure - clear vector
		try
		{
			SafeBulkConstruct(Position, Begin, End, Buffer);
		}
		catch (...)
		{
			// try to move values back
			try
			{
				SafeMoveBlock(Distance, Buffer + Position + Distance, Buffer + Position);
			}
			catch (...)
			{
				DestructRange(0, Position, Buffer);	// SafeMoveBlock destructed the rest
				Size = 0;
				AutoShrinkIfNeeded(EShrinkBehavior::Allow);
				throw;
			}
			throw;
		}
		Size += Distance;
	}


	template <typename T>
	template <typename IteratorType>
	void TVector<T>::SafeInsert(const size_t Position, const IteratorType Begin,
		const IteratorType End)
	{
		if (Position > Size)
		{
			throw  COutOfRange("Insert() vector out of range");
		}
		Insert(Position, Begin, End);
	}

	template <typename T>
	template <typename IteratorType>
	void TVector<T>::AutoInsert(const size_t Position, const IteratorType Begin,
		const IteratorType End, const T& DefaultValue)
	{
		if (Position > Size)
		{
			Resize(Position, DefaultValue, EShrinkBehavior::Allow);
		}
		Insert(Position, Begin, End);
	}


	template<typename T>
	void TVector<T>::Pop(const EShrinkBehavior ShrinkBehavior)
	{
		ASSERT(Size, "Pop() operation on empty vector");

		Destruct(Size - 1, Buffer);
		--Size;
		AutoShrinkIfNeeded(ShrinkBehavior);
	}


	template<typename T>
	void TVector<T>::SafePop(const EShrinkBehavior ShrinkBehavior)
	{
		if (!Size)
		{
			throw COutOfRange("Pop() operation on empty vector");
		}

		Pop(ShrinkBehavior);
	}


	template<typename T>
	T TVector<T>::SafePopGet(const EShrinkBehavior ShrinkBehavior)
	{
		if (!Size)
		{
			throw COutOfRange("Pop() operation on empty vector");
		}
		try
		{
			T PopValue(Move(Buffer[Size - 1]));	// may throw
			Pop(ShrinkBehavior);
			return PopValue;
		}
		catch (...)
		{
			// handles Move(Buffer[...]) exception
			// not the best solution, because vector may be cleared
			// twice, but move throw is not supposed to happen often
			Clear();
			throw;
		}
	}


	template<typename T>
	void TVector<T>::PopMultiple(const size_t ElementsToPop,
		const EShrinkBehavior ShrinkBehavior)
	{
		if (ElementsToPop >= Size)
		{
			Clear(ShrinkBehavior);
			return;
		}

		DestructRange(Size - ElementsToPop, Size, Buffer);
		Size -= ElementsToPop;

		AutoShrinkIfNeeded(ShrinkBehavior);
	}



	template<typename T>
	void TVector<T>::Shift(const EShrinkBehavior ShrinkBehavior)
	{
		ASSERT(Size, "Shift() operation on empty vector");

		Destruct(0, Buffer);

		try
		{
			SafeMoveBlock(Size - 1, Buffer + 1, Buffer);
		}
		catch (...)
		{
			Size = 0;
			AutoShrinkIfNeeded(ShrinkBehavior);
			throw;
		}
		--Size;

		AutoShrinkIfNeeded(ShrinkBehavior);
	}


	template<typename T>
	void TVector<T>::SafeShift(const EShrinkBehavior ShrinkBehavior)
	{
		if (!Size)
		{
			throw COutOfRange("Shift() operation on empty vector");
		}

		Shift(ShrinkBehavior);
	}


	template<typename T>
	T TVector<T>::SafeShiftGet(const EShrinkBehavior ShrinkBehavior)
	{
		if (!Size)
		{
			throw COutOfRange("Shift() operation on empty vector");
		}
		try
		{
			T ShiftValue(Move(Buffer[0]));	// may throw
			Shift(ShrinkBehavior);
			return ShiftValue;
		}
		catch (...)
		{
			// handles Move(Buffer[...]) exception
			// not the best solution, because vector may be cleared
			// twice, but move throw is not supposed to happen often
			Clear();
			throw;
		}
	}


	template<typename T>
	void TVector<T>::ShiftMultiple(const size_t ElementsToShift,
		const EShrinkBehavior ShrinkBehavior)
	{
		if (ElementsToShift >= Size)
		{
			Clear(ShrinkBehavior);
			return;
		}

		try
		{
			SafeMoveBlock(Size - ElementsToShift, Buffer + ElementsToShift, Buffer);
		}
		catch (...)
		{
			Size = 0;
			AutoShrinkIfNeeded(ShrinkBehavior);
			throw;
		}
		Size -= ElementsToShift;

		AutoShrinkIfNeeded(ShrinkBehavior);
	}



	template<typename T>
	void TVector<T>::Erase(const size_t Position, const EShrinkBehavior ShrinkBehavior)
	{
		ASSERT(Position < Size, "Erase() vector out of range");

		Destruct(Position, Buffer);

		try
		{
			SafeMoveBlock(Size - Position, Buffer + Position + 1, Buffer + Position);
		}
		catch (...)
		{
			DestructRange(0, Position, Buffer);
			Size = 0;
			AutoShrinkIfNeeded(ShrinkBehavior);
			throw;
		}
		Size--;

		AutoShrinkIfNeeded(ShrinkBehavior);
	}


	template<typename T>
	void TVector<T>::SafeErase(const size_t Position,
		const EShrinkBehavior ShrinkBehavior)
	{
		if (Position >= Size)
		{
			throw COutOfRange("Out of range: Erase() vector");
		}

		Erase(ShrinkBehavior);
	}


	template<typename T>
	T TVector<T>::SafeEraseGet(const size_t Position,
		const EShrinkBehavior ShrinkBehavior)
	{
		if (Position >= Size)
		{
			throw COutOfRange("Out of range: Erase() vector");
		}
		try
		{
			T EraseValue(Move(Buffer[Position]));	// may throw
			Erase(Position, ShrinkBehavior);
			return EraseValue;
		}
		catch (...)
		{
			// handles Move(Buffer[...]) exception
			// not the best solution, because vector may be cleared
			// twice, but move throw is not supposed to happen often
			Clear();
			throw;
		}
	}


	// Note: different name was chosen to avoid implicit conversion
	// of second parameter to boolean and deleting only one element
	template<typename T>
	void TVector<T>::EraseMultiple(const size_t PositionFrom,
		size_t PositionTo, const EShrinkBehavior ShrinkBehavior)
	{
		if (PositionTo > Size)
		{
			PositionTo = Size;
		}

		try
		{
			SafeMoveBlock(Size - PositionTo, Buffer + PositionTo,
				Buffer + PositionFrom);
		}
		catch (...)
		{
			DestructRange(0, PositionFrom, Buffer);
			Size = 0;
			AutoShrinkIfNeeded(ShrinkBehavior);
			throw;
		}
		Size -= PositionTo - PositionFrom;

		AutoShrinkIfNeeded(ShrinkBehavior);
	}



	template<typename T>
	void TVector<T>::Reserve(const size_t NewCapacity)
	{
		if (NewCapacity >= Size)
		{
			if (NewCapacity)
			{
				Reconstruct(Size, NewCapacity, Buffer, Size, Capacity);
			}
			else
			{
				Clear();
			}
		}
	}


	template<typename T>
	void TVector<T>::Resize(const size_t NewSize, const T& DefaultValue,
		const EShrinkBehavior ShrinkBehavior)
	{
		if (NewSize > Capacity)
		{
			Reconstruct(Size, CalcExtendedCapacity(NewSize),
				Buffer, Size, Capacity);
		}

		try
		{
			if (NewSize > Size)
			{
				SafeFillConstruct(Size, NewSize, Buffer, DefaultValue);
			}
			else
			{
				DestructRange(NewSize, Size, Buffer);
			}
		}
		catch (...)
		{
			AutoShrinkIfNeeded(ShrinkBehavior);
			throw;
		}

		Size = NewSize;
		AutoShrinkIfNeeded(ShrinkBehavior);
	}



	template<typename T>
	void TVector<T>::Swap(TVector<T>& Other) noexcept
	{
		::Swap(Other.Buffer, this->Buffer);
		::Swap(Other.Capacity, this->Capacity);
		::Swap(Other.Size, this->Size);
	}



	template<typename T>
	void TVector<T>::ShrinkToFit()
	{
		if (Size == Capacity)
		{
			return;
		}

		if (Size)
		{
			Reconstruct(Size, Size, Buffer, Size, Capacity);
		}
		else
		{
			Clear(EShrinkBehavior::Require);
		}
	}


	template<typename T>
	void TVector<T>::Clear(EShrinkBehavior ShrinkBehavior)
	{
		DestructAll(Size, Buffer);
		Size = 0;
		if (ShrinkBehavior == EShrinkBehavior::Require)
		{
			Deallocate(Buffer);
			Capacity = 0;
		}
		else
		{
			AutoShrinkIfNeeded(ShrinkBehavior);
		}
	}



	template<typename T>
	size_t TVector<T>::GetSize() const noexcept
	{
		return Size;
	}


	template<typename T>
	size_t TVector<T>::GetCapacity() const noexcept
	{
		return Capacity;
	}

	template<typename T>
	bool TVector<T>::IsEmpty() const noexcept
	{
		return Size == 0;
	}


	template<typename T>
	void TVector<T>::SetCapacityRule(
		const EReservedCapacityRule CapacityRule) noexcept
	{
		this->CapacityRule = CapacityRule;
	}


	template<typename T>
	typename TVector<T>::EReservedCapacityRule
		TVector<T>::GetCapacityRule() const noexcept
	{
		return CapacityRule;
	}



	template<typename T>
	T& TVector<T>::Front()
	{
		ASSERT(Size, "Front() operation on empty vector");

		return Buffer[0];
	}


	template<typename T>
	T& TVector<T>::SafeFront()
	{
		if (!Size)
		{
			throw COutOfRange("Front() operation on empty vector");
		}

		return Front();
	}


	template<typename T>
	T& TVector<T>::Back()
	{
		ASSERT(Size, "Back() operation on empty vector");

		return Buffer[Size - 1];
	}


	template<typename T>
	T& TVector<T>::SafeBack()
	{
		if (!Size)
		{
			throw COutOfRange("Back() operation on empty vector");
		}

		return Back();
	}



	template<typename T>
	size_t TVector<T>::CalcExtendedCapacity(const size_t NewSize)
	{
		// no switch case for optimization
		// as first condition is almost always true
		if (CapacityRule ==
			EReservedCapacityRule::Exponential)
		{	// Capacity is never calculated for size==0; omitting that
			return NewSize >= 2 ? (NewSize - 1) * 2 : 2;
		}
		if (CapacityRule ==
			EReservedCapacityRule::Linear)
		{
			return NewSize + 3 + 32 / sizeof(T);
		}
		return NewSize;
	}


	template<typename T>
	void TVector<T>::AutoShrinkIfNeeded(EShrinkBehavior ShrinkBehavior)
	{
		// no switch case for optimization
		// as first conditions are almost always true

		if (ShrinkBehavior == EShrinkBehavior::Allow)
		{

			if (CapacityRule == EReservedCapacityRule::Exponential)
			{
				// if capacity exceeds 4*size and is
				// not small, dealloc half of it. 
				if (Capacity >= 4 * Size &&
					Capacity >= 2 + 16 / sizeof(T))
				{
					Reconstruct(Size, Size * 2, Buffer, Size, Capacity);
				}
			}

			else if (CapacityRule == EReservedCapacityRule::Linear)
			{
				// if capacity exceeds size by > 2 chunks,
				// leave one empty chunk
				if (Capacity - Size >= 8 + 64 / sizeof(T))
				{
					Reconstruct(Size, Size + 4 + 32 / sizeof(T),
						Buffer, Size, Capacity);
				}
			}

			else
			{
				ShrinkToFit();
			}

		}

		else if (ShrinkBehavior == EShrinkBehavior::Require)
		{
			ShrinkToFit();
		}
	}

}