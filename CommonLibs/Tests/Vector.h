// RUN IN DEBUG MODE!!

#pragma once

#include "../CommonTypes/Vector.h"
#include "../CommonUtils/Assert.h"

using namespace Common;

namespace VectorTests
{

	void VectorTestConstructors()
	{
		TVector<int> First;
		ASSERT(First.GetSize() == 0 && First.GetCapacity() == 0, "Ctor error");

		TVector<char> Second(10);
		ASSERT(Second.GetSize() == 10 && Second[0] == 0 && Second[8] == 0, "Ctor error");

		TVector<char> Third(100, 2);
		ASSERT(Third.GetSize() == 100 && Third[0] == 2 && Third[99] == 2, "Ctor error");

		long long* HeapArray = new long long[10];
		HeapArray[0] = 0;
		HeapArray[3] = 3;
		HeapArray[9] = 9;
		TVector<long long> Fourth(10, HeapArray);
		ASSERT(Fourth.GetSize() == 10 && Fourth[0] == 0 && Fourth[3] == 3 && Fourth[9] == 9, "Ctor error");

		TVector<short int> Fifth = {1,2,3,4,5};
		ASSERT(Fifth.GetSize() == 5 && Fifth[0] == 1 && Fifth[2] == 3 && Fifth[4] == 5, "Ctor error");

		TVector<short int> Sixth(Fifth.Begin(), Fifth.End());
		ASSERT(Sixth == Fifth, "Ctor error");

		TVector<short int> Seventh(Fifth.Begin()+3, Fifth.End());
		ASSERT(Seventh.GetSize() == 2 && Seventh[0] == 4 && Seventh[1] == 5, "Ctor error");

		TVector<short int> Eighth(Fifth.Begin(), Fifth.Begin());
		ASSERT(First.GetSize() == 0 && First.GetCapacity() == 0, "Ctor error");

		TVector<short int> Nineth = Sixth;
		ASSERT(Nineth == Sixth && Nineth.GetCapacity() == Sixth.GetCapacity(), "Ctor error");

		TVector<int> Tenth = First;
		ASSERT(Tenth == First && Tenth.GetCapacity() == 0, "Ctor error");
	}

	void VectorTestOperators()
	{
		TVector<int> First = { 1,2,3,4,5 };
		TVector<int> Second = { 1,2,3,4,5 };
		Second.Reserve(500);
		First.Reserve(1);
		ASSERT(First == Second, "TestOps error");

		First[0] = 0;
		ASSERT(First != Second, "TestOps error");

		First[0] = 1;
		First[4] = 0;
		Second.ShrinkToFit();
		ASSERT(First != Second, "TestOps error");


		First.Clear();
		TVector<int> Third;
		ASSERT(First == Second, "TestOps error");

		ASSERT(Second[0] == 1 && Second[3] == 4 && Second[4] == 4, "TestOps error");

		bool bThrown = false;
		try
		{
			First.SafeAt(4);
		} 
		catch (...)
		{
			bThrown = true;
		}
		ASSERT(!bThrown, "TestOps error");

		try
		{
			First.SafeAt(5);
		}
		catch (...)
		{
			bThrown = true;
		}
		ASSERT(bThrown, "TestOps error");
	}

	void VectorTestAssignment()
	{

	}

	void VectorTestPushPopShift()
	{
		TVector<int> First = { 1,2,3,4,5 };
		TVector<int> Second = { 6,7,8,9 };
		TVector<int> Third = { 1,2,3,4,5,6,7,8,9 };
		First.Push(Second.Begin(), Second.End());
		ASSERT(First == Third, "Vector PushPopShift error");
	}

	void VectorTestInsert()
	{
		TVector<int> First = { 1,2,3,4,5 };
		TVector<int> Second = { 1,2, 99, 3,4,5 };
		First.Insert(2, 99);
		ASSERT(First == Second, "Vector insert error");

		First.Insert(0, 98);
		First.Insert(7, 100);
		Second = { 98,1,2,99,3,4,5, 100 };
		ASSERT(First == Second, "Vector insert error");
		
		First = { 1 };
		Second = { 1,0,0,0,0,0,0,0,0,99 };
		First.Insert(9, 99);
		ASSERT(First == Second, "Vector insert error");

		First = {};
		First.Insert(0, 1);
		Second = { 1 };
		ASSERT(First == Second, "Vector insert error");

	}

	void VectorTestReserve()
	{

	}

	void VectorTestResize()
	{
		TVector<int> First = { 1,2,3 };
		TVector<int> Second = { 1,2,3,0,0,0,0,0 };
		First.Resize(8);
		ASSERT(First == Second, "Vector resize error");

		First.Resize(1);
		Second = { 1 };
		ASSERT(First == Second, "Vector resize error");

		First.Resize(0);
		Second = {};
		ASSERT(First == Second, "Vector resize error");

		First = { 1,2,3 };
		Second = { 4,5,6 };
		First.Insert(1, Second.Begin(), Second.End());
		TVector<int> Third = { 1,4,5,6,2,3 };
		ASSERT(First == Third, "Vector resize error");

		First.Insert(16, Second.Begin(), Second.End());
		Third = { 1,4,5,6,2,3,0,0,0,0,0,0,0,0,0,0,4,5,6 };
		ASSERT(First == Third, "Vector resize error");

		First = {};
		First.Insert(0,Second.ConstBegin(), Second.ConstEnd());
		ASSERT(First == Second, "Vector resize error");
	}

	void VectorTestErase()
	{

	}

	void VectorTestIterators()
	{

	}

	void VectorTestConst()
	{
		const TVector<int> First = { 1,1,3,4,5 };
		ASSERT(First.Front() == First.SafeAt(1), "Vector const error");
		ASSERT(First.Back()-2 == First[2], "Vector const error");
		// no more tests needed thanks to IDE
	}


	void RunVectorTests()
	{
		VectorTestConstructors();
		VectorTestAssignment();
		VectorTestPushPopShift();
		VectorTestInsert();
		VectorTestReserve();
		VectorTestResize();
		VectorTestErase();
		VectorTestIterators();
		VectorTestConst();
	}

}