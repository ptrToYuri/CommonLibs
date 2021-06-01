// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

#include "./../Exception.h"

namespace Common
{

	namespace Iterators
	{

		template <typename NdPtrType, typename ValRefType>
		class TDoublyLinkedIterator
		{

		public:

			TDoublyLinkedIterator();
			TDoublyLinkedIterator(NdPtrType InitialPosition);

			const TDoublyLinkedIterator& operator ++ ();
			TDoublyLinkedIterator operator + (size_t Offset);
			const TDoublyLinkedIterator& operator += (size_t Offset);
			const TDoublyLinkedIterator& operator -- ();
			TDoublyLinkedIterator operator - (size_t Offset);
			const TDoublyLinkedIterator& operator -= (size_t Offset);
			bool operator == (const TDoublyLinkedIterator& Other);
			bool operator != (const TDoublyLinkedIterator& Other);

			ValRefType operator * ();

//		private:

			NdPtrType NodePointer;

		};


		template <typename NdPtrType, typename ValRefType>
		class TReverseDoublyLinkedIterator
		{

		public:

			TReverseDoublyLinkedIterator();
			TReverseDoublyLinkedIterator(NdPtrType InitialPosition);

			const TReverseDoublyLinkedIterator& operator ++ ();
			TReverseDoublyLinkedIterator operator + (size_t Offset);
			const TReverseDoublyLinkedIterator& operator += (size_t Offset);
			const TReverseDoublyLinkedIterator& operator -- ();
			TReverseDoublyLinkedIterator operator - (size_t Offset);
			const TReverseDoublyLinkedIterator& operator -= (size_t Offset);
			bool operator == (const TReverseDoublyLinkedIterator& Other);
			bool operator != (const TReverseDoublyLinkedIterator& Other);

			ValRefType operator * ();

//		private:

			NdPtrType NodePointer;

		};


		template <typename NdPtrType, typename ValRefType>
		class TSafeDoublyLinkedIterator {

		public:

			TSafeDoublyLinkedIterator();
			TSafeDoublyLinkedIterator(NdPtrType InitialPosition);

			const TSafeDoublyLinkedIterator& operator ++ ();
			TSafeDoublyLinkedIterator operator + (size_t Offset);
			const TSafeDoublyLinkedIterator& operator += (size_t Offset);
			const TSafeDoublyLinkedIterator& operator -- ();
			TSafeDoublyLinkedIterator operator - (size_t Offset);
			const TSafeDoublyLinkedIterator& operator -= (size_t Offset);
			bool operator == (const TSafeDoublyLinkedIterator& Other);
			bool operator != (const TSafeDoublyLinkedIterator& Other);

			ValRefType operator * ();

//		private:

			NdPtrType NodePointer;

		};


		template <class NdPtrType, typename ValRefType>
		class TSafeReverseDoublyLinkedIterator {

		public:

			TSafeReverseDoublyLinkedIterator();
			TSafeReverseDoublyLinkedIterator(NdPtrType InitialPosition);

			const TSafeReverseDoublyLinkedIterator& operator ++ ();
			TSafeReverseDoublyLinkedIterator operator + (size_t Offset);
			const TSafeReverseDoublyLinkedIterator& operator += (size_t Offset);
			const TSafeReverseDoublyLinkedIterator& operator -- ();
			TSafeReverseDoublyLinkedIterator operator - (size_t Offset);
			const TSafeReverseDoublyLinkedIterator& operator -= (size_t Offset);
			bool operator == (const TSafeReverseDoublyLinkedIterator& Other);
			bool operator != (const TSafeReverseDoublyLinkedIterator& Other);

			ValRefType operator * ();

//		private:

			NdPtrType NodePointer;

		};

	}

}

#include "../Private/Iterators/DoublyLinked.tpp"