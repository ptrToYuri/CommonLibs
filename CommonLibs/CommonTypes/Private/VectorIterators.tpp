namespace Common
{

	template <typename T>
	template <typename PointerType, typename ReferenceType>
	class TVector<T>::TIterator
	{

	public:

		TIterator(T* InitialPosition)
			: InternalPointer(InitialPosition) {}

		const TIterator& operator ++ ()
		{
			++InternalPointer;
			return *this;
		}

		TIterator operator + (unsigned int Offset)
		{
			return TIterator(InternalPointer + Offset);
		}

		const TIterator& operator += (unsigned int Offset)
		{
			InternalPointer += Offset;
			return *this;
		}

		const TIterator& operator -- ()
		{
			--InternalPointer;
			return *this;
		}

		TIterator operator - (unsigned int Offset)
		{
			return TIterator(InternalPointer - Offset);
		}

		const TIterator& operator -= (unsigned int Offset)
		{
			InternalPointer -= Offset;
			return *this;
		}

		bool operator == (const TIterator& Other)
		{
			return InternalPointer == Other.InternalPointer;
		}

		bool operator != (const TIterator& Other)
		{
			return InternalPointer != Other.InternalPointer;
		}

		ReferenceType operator * ()
		{
			return *InternalPointer;
		}

	private:

		PointerType InternalPointer;

	};



	template <typename T>
	template <typename PointerType, typename ReferenceType>
	class TVector<T>::TReverseIterator
	{

	public:

		TReverseIterator(T* InitialPosition)
			: InternalPointer(InitialPosition) {};

		const TReverseIterator& operator ++ ()
		{
			--InternalPointer;
			return *this;
		}

		TReverseIterator operator + (unsigned int Offset)
		{
			return TReverseIterator(InternalPointer - Offset);
		}

		const TReverseIterator& operator += (unsigned int Offset)
		{
			InternalPointer -= Offset;
			return *this;
		}

		const TReverseIterator& operator -- ()
		{
			++InternalPointer;
			return *this;
		}

		TReverseIterator operator - (unsigned int Offset)
		{
			return TReverseIterator(InternalPointer + Offset);
		}

		const TReverseIterator& operator -= (unsigned int Offset)
		{
			InternalPointer += Offset;
			return *this;
		}

		bool operator == (const TReverseIterator& Other)
		{
			return InternalPointer == Other.InternalPointer;
		}

		bool operator != (const TReverseIterator& Other)
		{
			return InternalPointer != Other.InternalPointer;
		}

		ReferenceType operator * ()
		{
			return *InternalPointer;
		}

	private:

		PointerType InternalPointer;

	};


	template<typename T>
	template <typename PointerType, typename ReferenceType>
	class TVector<T>::TSafeIterator {

	public:

		TSafeIterator(T* InitialPosition, const TVector<T>* Owner)
			: InternalPointer(InitialPosition), Owner(Owner) {};

		const TSafeIterator& operator ++ ()
		{
			if (InternalPointer == Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector iterator ++",
					InternalPointer + 1 - Buffer, Size);
			}
			++InternalPointer;
			return *this;
		}

		TSafeIterator operator + (unsigned int Offset)
		{
			if (InternalPointer + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector iterator +",
					InternalPointer + Offset - Buffer, Size);
			}
			return TSafeIterator(InternalPointer + Offset, Owner);
		}

		const TSafeIterator& operator += (unsigned int Offset)
		{
			if (InternalPointer + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector iterator +=",
					InternalPointer + Offset - Buffer, Size);
			}
			InternalPointer += Offset;
			return *this;
		}

		const TSafeIterator& operator -- ()
		{
			if (InternalPointer == Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector iterator --",
					InternalPointer - 1 - Buffer, Size);
			}
			--InternalPointer;
			return *this;
		}

		TSafeIterator operator - (unsigned int Offset)
		{
			if (InternalPointer - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector iterator -",
					InternalPointer - Offset - Buffer, Size);
			}
			return TSafeIterator(InternalPointer - Offset);
		}

		const TSafeIterator& operator -= (unsigned int Offset)
		{
			if (InternalPointer - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector iterator -=",
					InternalPointer - Offset - Buffer, Size);
			}
			InternalPointer -= Offset;
			return *this;
		}

		bool operator == (const TSafeIterator& Other)
		{
			return InternalPointer == Other.InternalPointer;
		}

		bool operator != (const TSafeIterator& Other)
		{
			return InternalPointer != Other.InternalPointer;
		}

		ReferenceType operator * ()
		{
			return *InternalPointer;
		}

	private:

		PointerType InternalPointer;
		TVector<T>* Owner;

	};



	template <typename T>
	template <typename PointerType, typename ReferenceType>
	class TVector<T>::TSafeReverseIterator
	{

	public:

		TSafeReverseIterator(T* InitialPosition, const TVector<T>* Owner)
			: InternalPointer(InitialPosition), Owner(Owner) {};

		const TSafeReverseIterator& operator ++ ()
		{
			if (InternalPointer == Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector reverse iterator ++",
					InternalPointer - 1 - Buffer, Size);
			}
			--InternalPointer;
			return *this;
		}

		TSafeReverseIterator operator + (unsigned int Offset)
		{
			if (InternalPointer - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector reverse iterator +",
					InternalPointer - Offset - Buffer, Size);
			}
			return TSafeReverseIterator(InternalPointer - Offset,
				Owner);
		}

		const TSafeReverseIterator& operator += (unsigned int Offset)
		{
			if (InternalPointer - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector reverse iterator +=",
					InternalPointer - Offset - Buffer, Size);
			}
			InternalPointer -= Offset;
			return *this;
		}

		const TSafeReverseIterator& operator -- ()
		{
			if (InternalPointer == Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector reverse iterator -- ",
					InternalPointer + 1 - Buffer, Size);
			}
			++InternalPointer;
			return *this;
		}

		TSafeReverseIterator operator - (unsigned int Offset)
		{
			if (InternalPointer + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector reverse iterator -",
					InternalPointer + Offset + Buffer, Size);
			}
			return TSafeReverseIterator(InternalPointer + Offset,
				Owner);
		}

		const TSafeReverseIterator& operator -= (unsigned int Offset)
		{
			if (InternalPointer + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector reverse iterator -=",
					InternalPointer + Offset + Buffer, Size);
			}
			InternalPointer += Offset;
			return *this;
		}

		bool operator == (const TSafeReverseIterator& Other)
		{
			return InternalPointer == Other.InternalPointer;
		}

		bool operator != (const TSafeReverseIterator& Other)
		{
			return InternalPointer != Other.InternalPointer;
		}

		ReferenceType operator * ()
		{
			return *InternalPointer;
		}

	private:

		PointerType InternalPointer;
		TVector<T>* Owner;

	};
	
}
