// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

#include "./../Exception.h"

namespace Common
{

	namespace Iterators
	{

		template <typename PtrType, typename RefType>
		class TBlockIterator
		{

		public:

			TBlockIterator();
			TBlockIterator(PtrType InitialPosition);

			const TBlockIterator& operator ++ ();
			TBlockIterator operator + (size_t Offset);
			const TBlockIterator& operator += (size_t Offset);
			const TBlockIterator& operator -- ();
			TBlockIterator operator - (size_t Offset);
			const TBlockIterator& operator -= (size_t Offset);
			bool operator == (const TBlockIterator& Other);
			bool operator != (const TBlockIterator& Other);

			RefType operator * ();

		private:

			PtrType InternalPointer;

		};


		template <typename PtrType, typename RefType>
		class TReverseBlockIterator
		{

		public:

			TReverseBlockIterator();
			TReverseBlockIterator(PtrType InitialPosition);

			const TReverseBlockIterator& operator ++ ();
			TReverseBlockIterator operator + (size_t Offset);
			const TReverseBlockIterator& operator += (size_t Offset);
			const TReverseBlockIterator& operator -- ();
			TReverseBlockIterator operator - (size_t Offset);
			const TReverseBlockIterator& operator -= (size_t Offset);
			bool operator == (const TReverseBlockIterator& Other);
			bool operator != (const TReverseBlockIterator& Other);

			RefType operator * ();

		private:

			PtrType InternalPointer;

		};


		template <typename PtrType, typename RefType, typename ContType>
		class TSafeBlockIterator {

		public:

			TSafeBlockIterator(PtrType InitialPosition, const ContType Owner);

			const TSafeBlockIterator& operator ++ ();
			TSafeBlockIterator operator + (size_t Offset);
			const TSafeBlockIterator& operator += (size_t Offset);
			const TSafeBlockIterator& operator -- ();
			TSafeBlockIterator operator - (size_t Offset);
			const TSafeBlockIterator& operator -= (size_t Offset);
			bool operator == (const TSafeBlockIterator& Other);
			bool operator != (const TSafeBlockIterator& Other);

			RefType operator * ();

		private:

			PtrType InternalPointer;
			const ContType Owner;

		};


		template <typename PtrType, typename RefType, class ContType>
		class TSafeReverseBlockIterator {

		public:

			TSafeReverseBlockIterator(PtrType InitialPosition, const ContType Owner);

			const TSafeReverseBlockIterator& operator ++ ();
			TSafeReverseBlockIterator operator + (size_t Offset);
			const TSafeReverseBlockIterator& operator += (size_t Offset);
			const TSafeReverseBlockIterator& operator -- ();
			TSafeReverseBlockIterator operator - (size_t Offset);
			const TSafeReverseBlockIterator& operator -= (size_t Offset);
			bool operator == (const TSafeReverseBlockIterator& Other);
			bool operator != (const TSafeReverseBlockIterator& Other);

			RefType operator * ();

		private:

			PtrType InternalPointer;
			const ContType Owner;

		};

	}

}

#include "../Private/Iterators/Block.tpp"