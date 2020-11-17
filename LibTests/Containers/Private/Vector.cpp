// Yuri Zamyatin, 2020. This file is part of LibTests

#include "../Vector.h"

using namespace Common;


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

	TVector<short int> Fifth = { 1,2,3,4,5 };
	ASSERT(Fifth.GetSize() == 5 && Fifth[0] == 1 && Fifth[2] == 3 && Fifth[4] == 5, "Ctor error");

	TVector<short int> Sixth(Fifth.Begin(), Fifth.End());
	ASSERT(Sixth == Fifth, "Ctor error");

	TVector<short int> Seventh(Fifth.Begin() + 3, Fifth.End());
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

	First = { 1,2,3 };
	First.AutoAt(9) = 9;
	ASSERT(First.AutoAt(0) == 1 && First.SafeAt(1) == 2 &&
		First[3] == 0 && First[9] == 9 && First.GetSize() == 10,
		"TestOps error");
}


void VectorTestAssignment()
{
	TVector<int> First;
	TVector<int> Second = { 1,2,3,4,5 };
	First.Assign(Second.Begin(), Second.End());
	ASSERT(First == Second, "Vector assignment error");

	Second = {};
	First.Assign(Second.ConstBegin(), Second.ConstEnd());
	ASSERT(First == Second, "Vector assignment error");

	Second = { 1,2,3 };
	First.Assign(Second.ConstBegin() + 1, Second.ConstEnd());
	Second.Shift();
	ASSERT(First == Second, "Vector assignment error");

	Second = { 1,2,3,4,5 };
	First = Second;
	ASSERT(First == Second, "Vector assignment error");
}


void VectorTestPushPopShift()
{
	TVector<int> First = { 1,2,3,4,5 };
	TVector<int> Second = { 6,7,8,9 };
	TVector<int> Third = { 1,2,3,4,5,6,7,8,9 };
	First.Push(Second.Begin(), Second.End());
	ASSERT(First == Third, "Vector PushPopShift error");

	First = {};
	First.Push(1);
	First.Push(2);
	First.Push(3);
	First.Push(4);
	First.ShiftMultiple(2, true);

	ASSERT(First.Shift() == 3 && First.Pop() == 4 && First.GetSize() == 0,
		"Vector PushPopShift error");

	First = { 1,2,3,4,5 };
	First.PopMultiple(12);
	First.PopMultiple(12);
	ASSERT(First.GetSize() == 0, "Vector PushPopShift error");

	First = { 1,2,3,4,5 };
	First.ShiftMultiple(12);
	First.ShiftMultiple(12);
	ASSERT(First.GetSize() == 0, "Vector PushPopShift error");

	bool bThrown = false;
	try
	{
		First.SafePop();
	}
	catch (COutOfRange&)
	{
		bThrown = true;
	}
	ASSERT(bThrown, "Vector PushPopShift error");

	bThrown = false;
	try
	{
		First.SafeShift();
	}
	catch (COutOfRange&)
	{
		bThrown = true;
	}
	ASSERT(bThrown, "Vector PushPopShift error");
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
	TVector<int> First;
	ASSERT(First.GetCapacity() == 0, "Vector capacity error");
	ASSERT(First.GetCapacityRule() ==
		TVector<int>::EReservedCapacityRule::Exponential,
		"Vector capacity error");

	First.Push(9);
	ASSERT(First.GetCapacity() == 2, "Vector capacity error");
	First.Push(9);

	First.Reserve(100);
	ASSERT(First.GetCapacity() == 100, "Vector capacity error");

	First.Reserve(98);
	ASSERT(First.GetCapacity() == 98 && First.GetSize() == 2,
		"Vector capacity error");

	First.Pop(true);
	ASSERT(First.GetCapacity() == 2, "Vector capacity error");

	First.Pop();
	First.ShrinkToFit();
	ASSERT(First.RawData() == nullptr && First.GetSize() == 0,
		"Vector capacity error");

	TVector<int> Second(TVector<int>::EReservedCapacityRule::NeverReserve);
	for (int i = 0; i < 50; ++i)
	{
		Second.Push(i);
		ASSERT(Second.GetSize() == Second.GetCapacity(), "Vector capacity error");
	}

	for (int i = 0; i < 50; ++i)
	{
		Second.Shift(true);
		ASSERT(Second.GetSize() == Second.GetCapacity(), "Vector capacity error");
	}
	ASSERT(Second.GetCapacity() == 0, "Vector capacity error");

	Second.SetCapacityRule(TVector<int>::EReservedCapacityRule::Linear);
	Second.Resize(64);
	ASSERT(Second.GetCapacity() == 75, "Vector capacity error");
	Second.Resize(15, 0, true);
	ASSERT(Second.GetCapacity() == 27, "Vector capacity error");

	Second.Reserve(30);
	ASSERT(Second.GetCapacity() == 30, "Vector capacity error");

	Second.Reserve(15);
	ASSERT(Second.GetCapacity() == 15, "Vector capacity error");

	Second.Reserve(0);
	ASSERT(Second.GetCapacity() == 15, "Vector capacity error");
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
	First.Insert(0, Second.ConstBegin(), Second.ConstEnd());
	ASSERT(First == Second, "Vector resize error");
}


void VectorTestErase()
{
	TVector<int> First = { 1,2,3,4,5 };
	First.Erase(0);
	First.Erase(1, true);
	TVector<int> Second = { 2,4,5 };
	ASSERT(First == Second, "Vector erase error");

	First.Erase(2);
	Second = { 2,4 };
	ASSERT(First == Second, "Vector erase error");

	First.Erase(0);
	First.Erase(0, true);
	Second = {};
	ASSERT(First == Second, "Vector erase error");

	First = { 1,2,3,4,5 };
	First.EraseMultiple(2, 10, true);
	Second = { 1,2 };
	ASSERT(First == Second, "Vector erase error");

	First = { 1,2,3,4,5 };
	First.EraseMultiple(0, 1);
	Second = { 2,3,4,5 };
	ASSERT(First == Second, "Vector erase error");

	First.EraseMultiple(0, 2);
	First.EraseMultiple(0, 0);
	First.EraseMultiple(0, 0);
	First.Erase(100);
	Second = { 4,5 };
	ASSERT(First == Second, "Vector erase error");

}


void VectorTestIterators()
{
	const TVector<int> First = { 1,2,3,4,5 };
	const TVector<int> Second = { 5,4,3,2,1 };
	const TVector<int> Third(First.SafeConstReverseBegin(),
		First.SafeConstReverseEnd());
	ASSERT(Third == Second, "Vector iterator error");

	int ErrIndex = 1000;
	size_t RangeMin = 1000, RangeMax = 1000;
	try
	{
		auto it = Third.SafeConstReverseBegin() + 6;
	}
	catch (const COutOfRange& Exception)
	{
		ErrIndex = Exception.GetRequestedIndex();
	}
	ASSERT(ErrIndex == -1, "Vector iterator error");

	try
	{
		auto it = Third.SafeConstReverseBegin() - 1;
	}
	catch (const COutOfRange& Exception)
	{
		ErrIndex = Exception.GetRequestedIndex();
	}
	ASSERT(ErrIndex == 5, "Vector iterator error");

	try
	{
		auto it = ++Third.SafeConstReverseEnd();
	}
	catch (const COutOfRange& Exception)
	{
		ErrIndex = Exception.GetRequestedIndex();
		RangeMin = Exception.GetExpectedRange().First;
		RangeMax = Exception.GetExpectedRange().Second;
		const char* const Message = Exception.GetMessage();
		ASSERT(AreRawStringsEqual(Message,
			"Out of range: vector rev. iterator ++"),
			"Vector iterator error");
	}
	ASSERT(ErrIndex == -1 && RangeMin == 0 && RangeMax == 5,
		"Vector iterator error");

	try
	{
		auto it = Third.SafeConstBegin() - 1;
	}
	catch (const COutOfRange& Exception)
	{
		ErrIndex = Exception.GetRequestedIndex();
		RangeMin = Exception.GetExpectedRange().First;
		RangeMax = Exception.GetExpectedRange().Second;
	}
	ASSERT(ErrIndex == -1 && RangeMin == 0 && RangeMax == 5,
		"Vector iterator error");

	try 
	{
		auto it = Third.SafeConstEnd() + 1;
	}
	catch (const COutOfRange& Exception)
	{
		ErrIndex = Exception.GetRequestedIndex();
		RangeMin = Exception.GetExpectedRange().First;
		RangeMax = Exception.GetExpectedRange().Second;
	}
	ASSERT(ErrIndex == 5 && RangeMin == 0 && RangeMax == 5,
		"Vector iterator error");


	for (const auto& el : Third);
	TVector<int> Fourth(Third);
	for (auto el : Third);
	for (const auto& el : Fourth);

	for (auto it = Fourth.Begin(); it != Fourth.End(); ++it)
	{
		*it += 2;
	}
	for (auto it = Fourth.ConstBegin(); it != Fourth.ConstEnd(); ++it);
	TVector<int> Fifth = { 7, 6, 5, 4, 3 };
	ASSERT(Fourth == Fifth, "Vector iterator error");

	for (auto it = Fourth.ReverseBegin(); it != Fourth.ReverseEnd(); ++it)
	{
		*it -= 2;
	}
	Fifth = { 5,4,3,2,1 };
	ASSERT(Fourth == Fifth, "Vector iterator error");

}


void VectorTestConst()
{
	const TVector<int> First = { 1,1,3,4,5 };
	ASSERT(First.Front() == First.SafeAt(1), "Vector const error");
	ASSERT(First.Back() - 2 == First[2], "Vector const error");
	// no more tests needed thanks to IDE
}