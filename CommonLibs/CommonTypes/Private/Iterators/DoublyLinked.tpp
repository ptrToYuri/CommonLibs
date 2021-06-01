// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

namespace Common
{

	namespace Iterators {

		// ================ TDoublyLinkedIterator =================

		template <class NdPtrType, typename ValRefType>
		TDoublyLinkedIterator<NdPtrType, ValRefType>::TDoublyLinkedIterator()
			: NodePointer(nullptr) {}

		template <class NdPtrType, typename ValRefType>
		TDoublyLinkedIterator<NdPtrType, ValRefType>::TDoublyLinkedIterator(NdPtrType InitialPosition)
			: NodePointer(InitialPosition) {}

		template <class NdPtrType, typename ValRefType>
		const TDoublyLinkedIterator<NdPtrType, ValRefType>& TDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator ++ ()
		{
			NodePointer = NodePointer->Next;
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		TDoublyLinkedIterator<NdPtrType, ValRefType> TDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator + (size_t Offset)
		{
			TDoublyLinkedIterator NewIterator = *this;
			NewIterator += Offset;
			return NewIterator;
		}

		template <class NdPtrType, typename ValRefType>
		const TDoublyLinkedIterator<NdPtrType, ValRefType>& TDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator += (size_t Offset)
		{
			for (size_t i = 0; i < Offset; ++i)
			{
				NodePointer = NodePointer->Next;
			}
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		const TDoublyLinkedIterator<NdPtrType, ValRefType>& TDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator -- ()
		{
			NodePointer = NodePointer->Previous;
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		TDoublyLinkedIterator<NdPtrType, ValRefType> TDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator - (size_t Offset)
		{
			TDoublyLinkedIterator NewIterator = *this;
			NewIterator -= Offset;
			return NewIterator;
		}

		template <class NdPtrType, typename ValRefType>
		const TDoublyLinkedIterator<NdPtrType, ValRefType>& TDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator -= (size_t Offset)
		{
			for (size_t i = 0; i < Offset; ++i)
			{
				NodePointer = NodePointer->Previous;
			}
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		bool TDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator == (const TDoublyLinkedIterator& Other)
		{
			return NodePointer == Other.NodePointer;
		}

		template <class NdPtrType, typename ValRefType>
		bool TDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator != (const TDoublyLinkedIterator& Other)
		{
			return !operator==(Other);
		}

		template <class NdPtrType, typename ValRefType>
		ValRefType TDoublyLinkedIterator<NdPtrType, ValRefType>::operator * ()
		{
			return NodePointer->Buffer;
		}


		// ================ TReverseDoublyLinkedIterator ================= //

		template <class NdPtrType, typename ValRefType>
		TReverseDoublyLinkedIterator<NdPtrType, ValRefType>::TReverseDoublyLinkedIterator()
			: NodePointer(nullptr) {}

		template <class NdPtrType, typename ValRefType>
		TReverseDoublyLinkedIterator<NdPtrType, ValRefType>::TReverseDoublyLinkedIterator
			(NdPtrType InitialPosition): NodePointer(InitialPosition) {}

		template <class NdPtrType, typename ValRefType>
		const TReverseDoublyLinkedIterator<NdPtrType, ValRefType>&
			TReverseDoublyLinkedIterator<NdPtrType, ValRefType>::operator ++ ()
		{
			NodePointer = NodePointer->Previous;
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		TReverseDoublyLinkedIterator<NdPtrType, ValRefType> TReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator + (size_t Offset)
		{
			TReverseDoublyLinkedIterator NewIterator = *this;
			NewIterator += Offset;
			return NewIterator;
		}

		template <class NdPtrType, typename ValRefType>
		const TReverseDoublyLinkedIterator<NdPtrType, ValRefType>& TReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator += (size_t Offset)
		{
			for (size_t i = 0; i < Offset; ++i)
			{
				NodePointer = NodePointer->Previous;
			}
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		const TReverseDoublyLinkedIterator<NdPtrType, ValRefType>& TReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator -- ()
		{
			NodePointer = NodePointer->Next;
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		TReverseDoublyLinkedIterator<NdPtrType, ValRefType> TReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator - (size_t Offset)
		{
			TReverseDoublyLinkedIterator NewIterator = *this;
			NewIterator -= Offset;
			return NewIterator;
		}

		template <class NdPtrType, typename ValRefType>
		const TReverseDoublyLinkedIterator<NdPtrType, ValRefType>& TReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator -= (size_t Offset)
		{
			for (size_t i = 0; i < Offset; ++i)
			{
				NodePointer = NodePointer->Next;
			}
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		bool TReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator == (const TReverseDoublyLinkedIterator& Other)
		{
			return NodePointer == Other.NodePointer;
		}

		template <class NdPtrType, typename ValRefType>
		bool TReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator != (const TReverseDoublyLinkedIterator& Other)
		{
			return !operator==(Other);
		}

		template <class NdPtrType, typename ValRefType>
		ValRefType TReverseDoublyLinkedIterator<NdPtrType, ValRefType>::operator * ()
		{
			return NodePointer->Buffer;
		}


		// ================ TSafeDoublyLinkedIterator ================= //

		
		template <class NdPtrType, typename ValRefType>
		TSafeDoublyLinkedIterator<NdPtrType, ValRefType>::TSafeDoublyLinkedIterator()
			: NodePointer(nullptr) {}

		template <class NdPtrType, typename ValRefType>
		TSafeDoublyLinkedIterator<NdPtrType, ValRefType>::TSafeDoublyLinkedIterator(NdPtrType InitialPosition)
			: NodePointer(InitialPosition) {}

		template <class NdPtrType, typename ValRefType>
		const TSafeDoublyLinkedIterator<NdPtrType, ValRefType>& TSafeDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator ++ ()
		{
			if (NodePointer->Next == nullptr)
			{
				throw COutOfRange("Out of range : DoublyLinkedIterator ++");
			}
			NodePointer = NodePointer->Next;
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		TSafeDoublyLinkedIterator<NdPtrType, ValRefType> TSafeDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator + (size_t Offset)
		{
			TSafeDoublyLinkedIterator NewIterator = *this;
			NewIterator += Offset;
			return NewIterator;
		}

		template <class NdPtrType, typename ValRefType>
		const TSafeDoublyLinkedIterator<NdPtrType, ValRefType>& TSafeDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator += (size_t Offset)
		{
			for (size_t i = 0; i < Offset; ++i)
			{
				if (NodePointer->Next == nullptr)
				{
					throw COutOfRange("Out of range : DoublyLinkedIterator +=");
				}
				NodePointer = NodePointer->Next;
			}
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		const TSafeDoublyLinkedIterator<NdPtrType, ValRefType>& TSafeDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator -- ()
		{
			if (NodePointer->Previous == nullptr)
			{
				throw COutOfRange("Out of range : DoublyLinkedIterator --");
			}
			NodePointer = NodePointer->Previous;
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		TSafeDoublyLinkedIterator<NdPtrType, ValRefType> TSafeDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator - (size_t Offset)
		{
			TSafeDoublyLinkedIterator NewIterator = *this;
			NewIterator -= Offset;
			return NewIterator;
		}

		template <class NdPtrType, typename ValRefType>
		const TSafeDoublyLinkedIterator<NdPtrType, ValRefType>& TSafeDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator -= (size_t Offset)
		{
			for (size_t i = 0; i < Offset; ++i)
			{
				if (NodePointer->Previous == nullptr)
				{
					throw COutOfRange("Out of range : DoublyLinkedIterator -=");
				}
				NodePointer = NodePointer->Previous;
			}
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		bool TSafeDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator == (const TSafeDoublyLinkedIterator& Other)
		{
			return NodePointer == Other.NodePointer;
		}

		template <class NdPtrType, typename ValRefType>
		bool TSafeDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator != (const TSafeDoublyLinkedIterator& Other)
		{
			return !operator==(Other);
		}

		template <class NdPtrType, typename ValRefType>
		ValRefType TSafeDoublyLinkedIterator<NdPtrType, ValRefType>::operator * ()
		{
			return NodePointer->Buffer;
		}


		// ================ TSafeReverseDoublyLinkedIterator ================= //

		template <class NdPtrType, typename ValRefType>
		TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>::TSafeReverseDoublyLinkedIterator()
			: NodePointer(nullptr) {}

		template <class NdPtrType, typename ValRefType>
		TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>::TSafeReverseDoublyLinkedIterator
		(NdPtrType InitialPosition) : NodePointer(InitialPosition) {}

		template <class NdPtrType, typename ValRefType>
		const TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>&
			TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>::operator ++ ()
		{
			if (NodePointer->Previous == nullptr)
			{
				throw COutOfRange("Out of range : rev. DoublyLinkedIterator ++");
			}
			NodePointer = NodePointer->Previous;
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType> TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator + (size_t Offset)
		{
			TSafeReverseDoublyLinkedIterator NewIterator = *this;
			NewIterator += Offset;
			return NewIterator;
		}

		template <class NdPtrType, typename ValRefType>
		const TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>&
			TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator += (size_t Offset)
		{
			for (size_t i = 0; i < Offset; ++i)
			{
				if (NodePointer->Previous == nullptr)
				{
					throw COutOfRange("Out of range : rev. DoublyLinkedIterator +=");
				}
				NodePointer = NodePointer->Previous;
			}
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		const TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>&
			TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator -- ()
		{
			if (NodePointer->Next == nullptr)
			{
				throw COutOfRange("Out of range : rev. DoublyLinkedIterator --");
			}
			NodePointer = NodePointer->Next;
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType> TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator - (size_t Offset)
		{
			TSafeReverseDoublyLinkedIterator NewIterator = *this;
			NewIterator -= Offset;
			return NewIterator;
		}

		template <class NdPtrType, typename ValRefType>
		const TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>& TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator -= (size_t Offset)
		{
			for (size_t i = 0; i < Offset; ++i)
			{
				if (NodePointer->Next == nullptr)
				{
					throw COutOfRange("Out of range : rev. DoublyLinkedIterator -=");
				}
				NodePointer = NodePointer->Next;
			}
			return *this;
		}

		template <class NdPtrType, typename ValRefType>
		bool TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator == (const TSafeReverseDoublyLinkedIterator& Other)
		{
			return NodePointer == Other.NodePointer;
		}

		template <class NdPtrType, typename ValRefType>
		bool TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>
			::operator != (const TSafeReverseDoublyLinkedIterator& Other)
		{
			return !operator==(Other);
		}

		template <class NdPtrType, typename ValRefType>
		ValRefType TSafeReverseDoublyLinkedIterator<NdPtrType, ValRefType>::operator * ()
		{
			return NodePointer->Buffer;
		}

	}

}