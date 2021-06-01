// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

namespace Common
{

	namespace Iterators {

		// ================ TBlockIterator =================

		template <typename PtrType, typename RefType>
		TBlockIterator<PtrType, RefType>::TBlockIterator()
			: InternalPointer(nullptr) {}

		template <typename PtrType, typename RefType>
		TBlockIterator<PtrType, RefType>::TBlockIterator(PtrType InitialPosition)
			: InternalPointer(InitialPosition) {}

		template <typename PtrType, typename RefType>
		const TBlockIterator<PtrType, RefType>& TBlockIterator<PtrType, RefType>
			::operator ++ ()
		{
			++InternalPointer;
			return *this;
		}

		template <typename PtrType, typename RefType>
		TBlockIterator<PtrType, RefType> TBlockIterator<PtrType, RefType>
			::operator + (size_t Offset)
		{
			return TBlockIterator(InternalPointer + Offset);
		}

		template <typename PtrType, typename RefType>
		const TBlockIterator<PtrType, RefType>& TBlockIterator<PtrType, RefType>
			::operator += (size_t Offset)
		{
			InternalPointer += Offset;
			return *this;
		}

		template <typename PtrType, typename RefType>
		const TBlockIterator<PtrType, RefType>& TBlockIterator<PtrType, RefType>
			::operator -- ()
		{
			--InternalPointer;
			return *this;
		}

		template <typename PtrType, typename RefType>
		TBlockIterator<PtrType, RefType> TBlockIterator<PtrType, RefType>
			::operator - (size_t Offset)
		{
			return TBlockIterator(InternalPointer - Offset);
		}

		template <typename PtrType, typename RefType>
		const TBlockIterator<PtrType, RefType>& TBlockIterator<PtrType, RefType>
			::operator -= (size_t Offset)
		{
			InternalPointer -= Offset;
			return *this;
		}

		template <typename PtrType, typename RefType>
		bool TBlockIterator<PtrType, RefType>
			::operator == (const TBlockIterator& Other)
		{
			return InternalPointer == Other.InternalPointer;
		}

		template <typename PtrType, typename RefType>
		bool TBlockIterator<PtrType, RefType>
			::operator != (const TBlockIterator& Other)
		{
			return InternalPointer != Other.InternalPointer;
		}

		template <typename PtrType, typename RefType>
		RefType TBlockIterator<PtrType, RefType>::operator * ()
		{
			return *InternalPointer;
		}


		// ================ TReverseBlockIterator ================= //

		template <typename PtrType, typename RefType>
		TReverseBlockIterator<PtrType, RefType>::TReverseBlockIterator()
			: InternalPointer(nullptr) {};

		template <typename PtrType, typename RefType>
		TReverseBlockIterator<PtrType, RefType>
			::TReverseBlockIterator(PtrType InitialPosition)
			: InternalPointer(InitialPosition - 1) {};

		template <typename PtrType, typename RefType>
		const TReverseBlockIterator<PtrType, RefType>&
			TReverseBlockIterator<PtrType, RefType>::operator ++ ()
		{
			--InternalPointer;
			return *this;
		}

		template <typename PtrType, typename RefType>
		TReverseBlockIterator<PtrType, RefType>
			TReverseBlockIterator<PtrType, RefType>::operator + (size_t Offset)
		{
			return TReverseBlockIterator(InternalPointer - Offset);
		}

		template <typename PtrType, typename RefType>
		const TReverseBlockIterator<PtrType, RefType>&
			TReverseBlockIterator<PtrType, RefType>::operator += (size_t Offset)
		{
			InternalPointer -= Offset;
			return *this;
		}

		template <typename PtrType, typename RefType>
		const TReverseBlockIterator<PtrType, RefType>&
			TReverseBlockIterator<PtrType, RefType>::operator -- ()
		{
			++InternalPointer;
			return *this;
		}

		template <typename PtrType, typename RefType>
		TReverseBlockIterator<PtrType, RefType>
			TReverseBlockIterator<PtrType, RefType>::operator - (size_t Offset)
		{
			return TReverseBlockIterator(InternalPointer + Offset);
		}

		template <typename PtrType, typename RefType>
		const TReverseBlockIterator<PtrType, RefType>&
			TReverseBlockIterator<PtrType, RefType>::operator -= (size_t Offset)
		{
			InternalPointer += Offset;
			return *this;
		}

		template <typename PtrType, typename RefType>
		bool TReverseBlockIterator<PtrType, RefType>
			::operator == (const TReverseBlockIterator& Other)
		{
			return InternalPointer == Other.InternalPointer;
		}

		template <typename PtrType, typename RefType>
		bool TReverseBlockIterator<PtrType, RefType>
			::operator != (const TReverseBlockIterator& Other)
		{
			return InternalPointer != Other.InternalPointer;
		}

		template <typename PtrType, typename RefType>
		RefType TReverseBlockIterator<PtrType, RefType>::operator * ()
		{
			return *InternalPointer;
		}


		// ================ TSafeBlockIterator ================= //

		template <typename PtrType, typename RefType, typename ContType>
		TSafeBlockIterator<PtrType, RefType, ContType>::
			TSafeBlockIterator(PtrType InitialPosition, const ContType Owner)
			: InternalPointer(InitialPosition), Owner(Owner) {};
			
		template <typename PtrType, typename RefType, typename ContType>
		const TSafeBlockIterator<PtrType, RefType, ContType>&
			TSafeBlockIterator<PtrType, RefType, ContType>::operator ++ ()
		{
			if (InternalPointer == Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: BlockIterator ++",
					static_cast<int>(InternalPointer - Owner->Buffer),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			++InternalPointer;
			return *this;
		}

		template <typename PtrType, typename RefType, typename ContType>
		TSafeBlockIterator<PtrType, RefType, ContType>
			TSafeBlockIterator<PtrType, RefType, ContType>::operator + (size_t Offset)
		{
			if (InternalPointer + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: BlockIterator +",
					static_cast<int>(InternalPointer - 1 - Owner->Buffer)
					+ static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			return TSafeBlockIterator(InternalPointer + Offset, Owner);
		}

		template <typename PtrType, typename RefType, typename ContType>
		const TSafeBlockIterator<PtrType, RefType, ContType>&
			TSafeBlockIterator<PtrType, RefType, ContType>::operator += (size_t Offset)
		{
			if (InternalPointer + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: BlockIterator +=",
					static_cast<int>(InternalPointer - 1 - Owner->Buffer)
					+ static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			InternalPointer += Offset;
			return *this;
		}

		template <typename PtrType, typename RefType, typename ContType>
		const TSafeBlockIterator<PtrType, RefType, ContType>&
			TSafeBlockIterator<PtrType, RefType, ContType>::operator -- ()
		{
			if (InternalPointer == Owner->Buffer)
			{
				throw COutOfRange("Out of range: BlockIterator --",
					static_cast<int>(InternalPointer - 1 - Owner->Buffer),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			--InternalPointer;
			return *this;
		}

		template <typename PtrType, typename RefType, typename ContType>
		TSafeBlockIterator<PtrType, RefType, ContType>
			TSafeBlockIterator<PtrType, RefType, ContType>::operator - (size_t Offset)
		{
			if (InternalPointer - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: BlockIterator -",
					static_cast<int>(InternalPointer - Owner->Buffer)
					- static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			return TSafeBlockIterator(InternalPointer - Offset, Owner);
		}

		template <typename PtrType, typename RefType, typename ContType>
		const TSafeBlockIterator<PtrType, RefType, ContType>&
			TSafeBlockIterator<PtrType, RefType, ContType>::operator -= (size_t Offset)
		{
			if (InternalPointer - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: BlockIterator -=",
					static_cast<int>(InternalPointer - Owner->Buffer)
					- static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			InternalPointer -= Offset;
			return *this;
		}

		template <typename PtrType, typename RefType, typename ContType>
		bool TSafeBlockIterator<PtrType, RefType, ContType>
			::operator == (const TSafeBlockIterator& Other)
		{
			return InternalPointer == Other.InternalPointer;
		}

		template <typename PtrType, typename RefType, typename ContType>
		bool TSafeBlockIterator<PtrType, RefType, ContType>
			::operator != (const TSafeBlockIterator& Other)
		{
			return InternalPointer != Other.InternalPointer;
		}

		template <typename PtrType, typename RefType, typename ContType>
		RefType TSafeBlockIterator<PtrType, RefType, ContType>::operator * ()
		{
			return *InternalPointer;
		}


		// ================ TSafeReverseBlockIterator ================= //

		template <typename PtrType, typename RefType, class ContType>
		TSafeReverseBlockIterator<PtrType, RefType, ContType>
			::TSafeReverseBlockIterator(PtrType InitialPosition, const ContType Owner)
			: InternalPointer(InitialPosition - 1), Owner(Owner) {};

		template <typename PtrType, typename RefType, class ContType>
		const TSafeReverseBlockIterator<PtrType, RefType, ContType>&
			TSafeReverseBlockIterator<PtrType, RefType, ContType>
			::operator ++ ()
		{
			if (InternalPointer + 1 == Owner->Buffer)
			{
				throw COutOfRange("Out of range: rev. BlockIterator ++",
					static_cast<int>(InternalPointer - Owner->Buffer),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			--InternalPointer;
			return *this;
		}

		template <typename PtrType, typename RefType, class ContType>
		TSafeReverseBlockIterator<PtrType, RefType, ContType>
			TSafeReverseBlockIterator<PtrType, RefType, ContType>
			::operator + (size_t Offset)
		{
			if (InternalPointer + 1 - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: rev. BlockIterator +",
					static_cast<int>(InternalPointer + 1 - Owner->Buffer)
					- static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			return TSafeReverseBlockIterator(InternalPointer - Offset, Owner);
		}

		template <typename PtrType, typename RefType, class ContType>
		const TSafeReverseBlockIterator<PtrType, RefType, ContType>&
			TSafeReverseBlockIterator<PtrType, RefType, ContType>
			::operator += (size_t Offset)
		{
			if (InternalPointer + 1 - Offset < Owner->Buffer)
			{
				throw COutOfRange("Out of range: rev. BlockIterator +=",
					static_cast<int>(InternalPointer + 1 - Owner->Buffer)
					- static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			InternalPointer -= Offset;
			return *this;
		}

		template <typename PtrType, typename RefType, class ContType>
		const TSafeReverseBlockIterator<PtrType, RefType, ContType>&
			TSafeReverseBlockIterator<PtrType, RefType, ContType>
			::operator -- ()
		{
			if (InternalPointer + 1 == Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: rev. BlockIterator --",
					static_cast<int>(InternalPointer + 1 - Owner->Buffer),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			++InternalPointer;
			return *this;
		}

		template <typename PtrType, typename RefType, class ContType>
		TSafeReverseBlockIterator<PtrType, RefType, ContType>
			TSafeReverseBlockIterator<PtrType, RefType, ContType>
			::operator - (size_t Offset)
		{
			if (InternalPointer + 1 + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: rev. BlockIterator -",
					static_cast<int>(InternalPointer - Owner->Buffer)
					+ static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			return TSafeReverseBlockIterator(InternalPointer + Offset, Owner);
		}

		template <typename PtrType, typename RefType, class ContType>
		const TSafeReverseBlockIterator<PtrType, RefType, ContType>&
			TSafeReverseBlockIterator<PtrType, RefType, ContType>
			::operator -= (size_t Offset)
		{
			if (InternalPointer + 1 + Offset > Owner->Buffer + Owner->Size)
			{
				throw COutOfRange("Out of range: rev. BlockIterator -=",
					static_cast<int>(InternalPointer - Owner->Buffer)
					+ static_cast<int>(Offset),
					TPair<size_t, size_t>(0, Owner->Size));
			}
			InternalPointer += Offset;
			return *this;
		}

		template <typename PtrType, typename RefType, class ContType>
		bool TSafeReverseBlockIterator<PtrType, RefType, ContType>
			::operator == (const TSafeReverseBlockIterator& Other)
		{
			return InternalPointer == Other.InternalPointer;
		}

		template <typename PtrType, typename RefType, class ContType>
		bool TSafeReverseBlockIterator<PtrType, RefType, ContType>
			::operator != (const TSafeReverseBlockIterator& Other)
		{
			return InternalPointer != Other.InternalPointer;
		}

		template <typename PtrType, typename RefType, class ContType>
		RefType TSafeReverseBlockIterator<PtrType, RefType, ContType>::operator * ()
		{
			return *InternalPointer;
		}

	}

}