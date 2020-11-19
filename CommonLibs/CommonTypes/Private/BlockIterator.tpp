// Yuri Zamyatin, 2020. This file is part of CommonLibs

namespace Common
{

	template <typename T>
	template <typename PointerType, typename ReferenceType>
	class TVector<T>::TIterator
	{

	public:

		TIterator(PointerType InitialPosition)
			: InternalPointer(InitialPosition) {}

		const TIterator& operator ++ ()
		{
			++InternalPointer;
			return *this;
		}

		TIterator operator + (size_t Offset)
		{
			return TIterator(InternalPointer + Offset);
		}

		const TIterator& operator += (size_t Offset)
		{
			InternalPointer += Offset;
			return *this;
		}

		const TIterator& operator -- ()
		{
			--InternalPointer;
			return *this;
		}

		TIterator operator - (size_t Offset)
		{
			return TIterator(InternalPointer - Offset);
		}

		const TIterator& operator -= (size_t Offset)
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

		TReverseIterator(PointerType InitialPosition)
			: InternalPointer(InitialPosition - 1) {};

		const TReverseIterator& operator ++ ()
		{
			--InternalPointer;
			return *this;
		}

		TReverseIterator operator + (size_t Offset)
		{
			return TReverseIterator(InternalPointer - Offset);
		}

		const TReverseIterator& operator += (size_t Offset)
		{
			InternalPointer -= Offset;
			return *this;
		}

		const TReverseIterator& operator -- ()
		{
			++InternalPointer;
			return *this;
		}

		TReverseIterator operator - (size_t Offset)
		{
			return TReverseIterator(InternalPointer + Offset);
		}

		const TReverseIterator& operator -= (size_t Offset)
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

		TSafeIterator(PointerType InitialPosition, const TVector<T>* Owner)
			: InternalPointer(InitialPosition), Owner(Owner) {};

		const TSafeIterator& operator ++ ()
		{
			if (InternalPointer == Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector iterator ++",
					static_cast<int>(InternalPointer - Owner->Buffer),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			++InternalPointer;
			return *this;
		}

		TSafeIterator operator + (size_t Offset)
		{
			if (InternalPointer + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector iterator +",
					static_cast<int>(InternalPointer - 1 - Owner->Buffer)
					+ static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			return TSafeIterator(InternalPointer + Offset, Owner);
		}

		const TSafeIterator& operator += (size_t Offset)
		{
			if (InternalPointer + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector iterator +=",
					static_cast<int>(InternalPointer - 1 - Owner->Buffer)
					+ static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			InternalPointer += Offset;
			return *this;
		}

		const TSafeIterator& operator -- ()
		{
			if (InternalPointer == Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector iterator --",
					static_cast<int>(InternalPointer - 1 - Owner->Buffer),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			--InternalPointer;
			return *this;
		}

		TSafeIterator operator - (size_t Offset)
		{
			if (InternalPointer - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector iterator -",
					static_cast<int>(InternalPointer - Owner->Buffer)
					- static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			return TSafeIterator(InternalPointer - Offset, Owner);
		}

		const TSafeIterator& operator -= (size_t Offset)
		{
			if (InternalPointer - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector iterator -=",
					static_cast<int>(InternalPointer - Owner->Buffer)
					- static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
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
		const TVector<T>* const Owner;

	};



	template <typename T>
	template <typename PointerType, typename ReferenceType>
	class TVector<T>::TSafeReverseIterator
	{

	public:

		TSafeReverseIterator(PointerType InitialPosition, const TVector<T>* Owner)
			: InternalPointer(InitialPosition - 1), Owner(Owner) {};

		const TSafeReverseIterator& operator ++ ()
		{
			if (InternalPointer + 1 == Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector rev. iterator ++",
					static_cast<int>(InternalPointer - Owner->Buffer),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			--InternalPointer;
			return *this;
		}

		TSafeReverseIterator operator + (size_t Offset)
		{
			if (InternalPointer + 1 - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector rev. iterator +",
					static_cast<int>(InternalPointer + 1 - Owner->Buffer)
					- static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			return TSafeReverseIterator(InternalPointer - Offset, Owner);
		}

		const TSafeReverseIterator& operator += (size_t Offset)
		{
			if (InternalPointer + 1 - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: vector rev. iterator +=",
					static_cast<int>(InternalPointer + 1 - Owner->Buffer)
					- static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			InternalPointer -= Offset;
			return *this;
		}

		const TSafeReverseIterator& operator -- ()
		{
			if (InternalPointer + 1 == Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector rev. iterator --",
					static_cast<int>(InternalPointer + 1 - Owner->Buffer),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			++InternalPointer;
			return *this;
		}

		TSafeReverseIterator operator - (size_t Offset)
		{
			if (InternalPointer + 1 + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector rev. iterator -",
					static_cast<int>(InternalPointer - Owner->Buffer)
					+ static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			return TSafeReverseIterator(InternalPointer + Offset, Owner);
		}

		const TSafeReverseIterator& operator -= (size_t Offset)
		{
			if (InternalPointer + 1 + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: vector rev. iterator -=",
					static_cast<int>(InternalPointer - Owner->Buffer)
					+ static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
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
		const TVector<T>* const Owner;

	};

}