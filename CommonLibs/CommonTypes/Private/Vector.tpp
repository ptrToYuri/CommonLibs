// Yuri Zamyatin, 2020. This file is part of CommonLibs

namespace Common
{

	template<typename T>
	TVector<T>::TVector(const EReservedCapacityRule CapacityRule) noexcept
		: CapacityRule(CapacityRule) {};


	template<typename T>
	TVector<T>::TVector(const size_t Size, const T& DefaultValue)
		: Size(Size), Capacity(Size)
	{
		if (!Size)
		{
			return;
		}

		Buffer = new T[Capacity];
		for (size_t i = 0; i < Size; ++i)
		{
			Buffer[i] = DefaultValue;
		}

	}


	template<typename T>
	TVector<T>::TVector(const size_t Size, const T* const Array)
		: Size(Size), Capacity(Size)
	{
		if (!Size)
		{
			return;
		}

		Buffer = new T[Capacity];
		CopyFromArray(Size, Array, Buffer);

	}


	template<typename T>
	TVector<T>::TVector(const std::initializer_list<T>& ValuesList)
		: Size(ValuesList.size()), Capacity(Size)
	{
		if (!Size)
		{
			return;
		}

		Buffer = new T[Capacity];
		CopyFromIterators(ValuesList.begin(), ValuesList.end(), Buffer);

	}


	template <typename T>
	template <typename IteratorType>
	TVector<T>::TVector(const IteratorType Begin, const IteratorType End,
		// disable this constructor when it "wins" the first one
		typename std::enable_if<!std::is_integral<
		IteratorType>::value>::type*)

		: Size(GetIteratorDistance(Begin, End)), Capacity(Size)
	{
		if (!Size)
		{
			return;
		}

		Buffer = new T[Capacity];
		CopyFromIterators(Begin, End, Buffer);

	}


	template<typename T>
	TVector<T>::TVector(const TVector<T>& Other)
		: Size(Other.Size), Capacity(Other.Capacity),
		CapacityRule(Other.CapacityRule)
	{
		if (!Capacity)
		{
			return;
		}

		Buffer = new T[Capacity];
		CopyFromArray(Size, Other.Buffer, Buffer);

	}


	template<typename T>
	TVector<T>::TVector(TVector<T>&& Other) noexcept
		: Size(Other.Size), Capacity(Other.Capacity),
		CapacityRule(Other.CapacityRule), Buffer(Other.Buffer)
	{
		Other.Buffer = nullptr;
	}


	template<typename T>
	TVector<T>::~TVector()
	{
		delete[] Buffer;
	}



	template<typename T>
	template<typename IteratorType>
	void TVector<T>::Assign(const IteratorType Begin,
		const IteratorType End, const bool bAllowAutoShrink)
	{
		Size = GetIteratorDistance(Begin, End);

		if (Size > Capacity)
		{
			delete[] Buffer;	// empty buffer must be nullptr
			Capacity = CalcExtendedCapacity(Size);
			Buffer = AllocateOrResetAndThrow(Capacity);
		}
		else if (bAllowAutoShrink)
		{
			AutoShrinkIfNeeded();
		}

		CopyFromIterators(Begin, End, Buffer);

	}


	template <typename T>
	TVector<T>& TVector<T>::operator = (const
		std::initializer_list<T>& ValuesList)
	{
		Size = ValuesList.size();

		if (Size > Capacity)
		{
			delete[] Buffer;	// empty buffer must be nullptr
			Capacity = CalcExtendedCapacity(Size);
			Buffer = AllocateOrResetAndThrow(Capacity);
		}

		CopyFromIterators(ValuesList.begin(), ValuesList.end(), Buffer);

		return *this;
	}


	template<typename T>
	TVector<T>& TVector<T>::operator = (const TVector<T>& Other)
	{
		// user expects the same vector: everything must match
		if (Size != Other.Size || Capacity != Other.Capacity)
		{
			Size = Other.Size;
			Capacity = Other.Capacity;
			delete[] Buffer;
			if (Capacity)
			{
				Buffer = AllocateOrResetAndThrow(Capacity);
			}
			else
			{
				Buffer = nullptr;
			}
		}

		CopyFromArray(Size, Other.Buffer, Buffer);
		CapacityRule = Other.CapacityRule;

		return *this;
	}


	template<typename T>
	TVector<T>& TVector<T>::operator = (TVector<T>&& Other) noexcept
	{
		Size = Other.Size;
		Capacity = Other.Capacity;
		CapacityRule = Other.CapacityRule;
		Buffer = Other.Buffer;

		Other.Buffer = nullptr;

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
			Resize(Index + 1, DefaultValue, false);
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



	template <typename T>
	void TVector<T>::Push(const T& Value)
	{
		if (Size + 1 > Capacity)
		{
			Reallocate(Size, CalcExtendedCapacity(Size + 1));
		}

		Buffer[Size] = Value;

		++Size;
	}


	template <typename T>
	template <typename IteratorType>
	void TVector<T>::Push(const IteratorType Begin, const IteratorType End)
	{
		size_t Distance = GetIteratorDistance(Begin, End);

		if (Size + Distance > Capacity)
		{
			Reallocate(Size, CalcExtendedCapacity(Size + Distance));
		}

		CopyFromIterators(Begin, End, Buffer + Size);

		Size += Distance;
	}



	template <typename T>
	void TVector<T>::Insert(const size_t Position, const T& Value,
		const T& FillOnResizeWith)
	{
		size_t FutureCapacity = Capacity; // update capacity after new as it may throw
		if (Size >= Capacity || Position >= Capacity)
		{
			FutureCapacity = CalcExtendedCapacity(
				(Position > Size ? Position : Size) + 1);
		}
		T* Temp = new T[FutureCapacity];
		Capacity = FutureCapacity;

		if (Position <= Size)	// no fill required
		{

			CopyFromArray(Position, Buffer, Temp);
			CopyFromArray(Size - Position, Buffer + Position, Temp + Position + 1);
			++Size;

		}
		else		// fill with provided elements
		{

			CopyFromArray(Size, Buffer, Temp);
			for (size_t i = Size; i < Position; ++i)
			{
				Temp[i] = FillOnResizeWith;
			}
			Size = Position + 1;

		}

		Temp[Position] = Value;

		delete[] Buffer;
		Buffer = Temp;
	}


	template <typename T>
	template <typename IteratorType>
	void TVector<T>::Insert(const size_t Position, const IteratorType Begin,
		const IteratorType End, const T& FillOnResizeWith,
		// disable this constructor when it is not expected
		typename std::enable_if<!std::is_integral<
		IteratorType>::value>::type*)
	{
		size_t Distance = GetIteratorDistance(Begin, End);

		if (Distance == 0)
		{
			return;
		}

		size_t FutureCapacity = Capacity; // update capacity after new as it may throw
		if (Size + Distance > Capacity || Position + Distance > Capacity)
		{
			FutureCapacity = CalcExtendedCapacity(
				(Position > Size ? Position : Size) + Distance);
		}
		T* Temp = new T[FutureCapacity];
		Capacity = FutureCapacity;

		if (Position <= Size)	// no fill required
		{

			CopyFromArray(Position, Buffer, Temp);
			CopyFromArray(Size - Position, Buffer + Position, Temp + Position + Distance);
			CopyFromIterators(Begin, End, Temp + Position);
			Size += Distance;

		}
		else		// fill with provided elements
		{

			CopyFromArray(Size, Buffer, Temp);
			for (size_t i = Size; i < Position; ++i)
			{
				Temp[i] = FillOnResizeWith;
			}
			CopyFromIterators(Begin, End, Temp + Position);
			Size = Position + Distance;

		}

		delete[] Buffer;
		Buffer = Temp;
	}



	template<typename T>
	T TVector<T>::Pop(const bool bAllowAutoShrink)
	{
		ASSERT(Size, "Pop() operation on empty vector");

		T PopValue = Buffer[Size - 1];
		--Size;

		if (bAllowAutoShrink)
		{
			AutoShrinkIfNeeded();
		}

		return PopValue;
	}


	template<typename T>
	T TVector<T>::SafePop(const bool bAllowAutoShrink)
	{
		if (!Size)
		{
			throw COutOfRange("Pop() operation on empty vector");
		}

		return Pop(bAllowAutoShrink);
	}


	template<typename T>
	void TVector<T>::PopMultiple(const size_t ElementsToPop,
		const bool bAllowAutoShrink)
	{
		if (ElementsToPop >= Size)
		{
			Size = 0;
		}
		else
		{
			Size -= ElementsToPop;
		}

		if (bAllowAutoShrink)
		{
			AutoShrinkIfNeeded();
		}
	}



	template<typename T>
	T TVector<T>::Shift(const bool bAllowAutoShrink)
	{
		ASSERT(Size, "Shift() operation on empty vector");

		T ShiftValue = Buffer[0];

		--Size;
		CopyFromArray(Size, Buffer + 1, Buffer);

		if (bAllowAutoShrink)
		{
			AutoShrinkIfNeeded();
		}

		return ShiftValue;
	}


	template<typename T>
	T TVector<T>::SafeShift(const bool bAllowAutoShrink)
	{
		if (!Size)
		{
			throw COutOfRange("Shift() operation on empty vector");
		}

		return Shift(bAllowAutoShrink);
	}


	template<typename T>
	void TVector<T>::ShiftMultiple(const size_t ElementsToShift,
		const bool bAllowAutoShrink)
	{
		if (ElementsToShift >= Size)
		{
			Size = 0;
		}
		else
		{
			CopyFromArray(Size - ElementsToShift, Buffer + ElementsToShift, Buffer);
			Size -= ElementsToShift;
		}

		if (bAllowAutoShrink)
		{
			AutoShrinkIfNeeded();
		}
	}



	template<typename T>
	void TVector<T>::Erase(const size_t Position, const bool bAllowAutoShrink)
	{
		if (Position >= Size)
		{
			return;
		}

		CopyFromArray(Size - Position, Buffer + Position + 1, Buffer + Position);
		Size--;

		if (bAllowAutoShrink)
		{
			AutoShrinkIfNeeded();
		}
	}


	// note: different name was chosen intentionally to avoid implicit conversion
	// of second parameter to boolean and deleting only one element
	template<typename T>
	void TVector<T>::EraseMultiple(const size_t PositionFrom,
		size_t PositionTo, const bool bAllowAutoShrink)
	{
		if (PositionTo > Size)
		{
			PositionTo = Size;
		}
		size_t Offset = PositionTo - PositionFrom;
		if (Offset < 0)
		{
			return;
		}

		CopyFromArray(Size - PositionTo, Buffer + PositionFrom + Offset,
			Buffer + PositionFrom);
		Size -= Offset;

		if (bAllowAutoShrink)
		{
			AutoShrinkIfNeeded();
		}
	}



	template<typename T>
	void TVector<T>::Reserve(const size_t NewCapacity)
	{
		if (NewCapacity >= Size)
		{
			if (NewCapacity)
			{
				Reallocate(Size, NewCapacity);
			}
			else
			{
				Clear(true);
			}
		}
	}


	template<typename T>
	void TVector<T>::Resize(const size_t NewSize, const T& DefaultValue,
		const bool bAllowAutoShrink)
	{
		if (NewSize > Capacity)
		{
			Reallocate(Size, CalcExtendedCapacity(NewSize));
		}

		if (NewSize > Size)
		{
			for (size_t i = Size; i < NewSize; ++i)
			{
				Buffer[i] = DefaultValue;
			}
		}

		else if (bAllowAutoShrink)
		{
			Size = NewSize;
			AutoShrinkIfNeeded();
		}

		Size = NewSize;
	}



	template<typename T>
	void TVector<T>::Swap(TVector<T>& Vector1, TVector<T>& Vector2) noexcept
	{
		size_t Temp;

		Temp = Vector2.Size;
		Vector2.Size = Vector1.Size;
		Vector1.Size = Temp;

		Temp = Vector2.Capacity;
		Vector2.Capacity = Vector1.Capacity;
		Vector1.Capacity = Temp;

		T* TempBuffer = Vector2.Buffer;
		Vector2.Buffer = Vector2.Buffer;
		Vector1.Buffer = Temp;
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
			Reallocate(Size, Size);
		}
		else
		{
			Clear(true);
		}
	}


	template<typename T>
	void TVector<T>::Clear(bool bDoFreeMemory)
	{
		Size = 0;

		if (bDoFreeMemory)
		{
			delete[] Buffer;
			Buffer = nullptr;
			Capacity = 0;
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
		return CSafeIterator(Buffer);
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


	template <typename T>
	void TVector<T>::Reallocate(const size_t OldSize, const size_t NewCapacity)
	{
		T* Temp = new T[NewCapacity];
		Capacity = NewCapacity;

		CopyFromArray(OldSize, Buffer, Temp);

		delete[] Buffer;
		Buffer = Temp;
	}


	template<typename T>
	inline T* TVector<T>::AllocateOrResetAndThrow(const size_t NewCapacity)
	{
		try {
			return new T[NewCapacity];
		}
		catch (const std::bad_alloc& Exception)
		{
			Capacity = 0;
			Size = 0;
			throw Exception;
		}
	}


	template<typename T>
	void TVector<T>::AutoShrinkIfNeeded()
	{
		// no switch case for optimization
		// as first condition is almost always true
		if (CapacityRule ==
			EReservedCapacityRule::Exponential)
		{
			// if capacity exceeds 4*size and is
			// not small, dealloc half of it. 
			if (Capacity >= 4 * Size &&
				Capacity >= 2 + 16 / sizeof(T))
			{
				Reallocate(Size, Size * 2);
			}
		}

		else if (CapacityRule ==
			EReservedCapacityRule::Linear)
		{
			// if capacity exceeds size by > 2 chunks,
			// leave one empty chunk
			if (Capacity - Size >= 8 + 64 / sizeof(T))
			{
				Reallocate(Size, Size + 4 + 32 / sizeof(T));
			}
		}

		else
		{
			ShrinkToFit();
		}
	}



	template <typename T>
	template <typename IteratorType>
	void TVector<T>::CopyFromIterators(IteratorType Begin,
		const IteratorType End, T* outBuffer)
	{
		size_t i = 0;
		while (Begin != End)
		{
			outBuffer[i] = *Begin;
			++Begin;
			++i;
		}
	}


	template <typename T>
	inline void TVector<T>::CopyFromArray(const size_t Size,
		const T* const ArrayFrom, T* outArrayTo)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			outArrayTo[i] = ArrayFrom[i];
		}
	}

}