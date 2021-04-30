// Yuri Zamyatin, 2020-2021. This file is part of LibTests

#include "..\Optional.h"

using namespace Common;


// This should definitely be improved

class CTestClass
{
public:

	CTestClass() = delete;
	CTestClass(const CTestClass& Other)
		:A(Other.A)
	{
	//	std::cout << "Constructed copy\n";
	}
	CTestClass(int A)
		:A(A)
	{
	//	std::cout << "Int constructed: " << A << "\n";
	}
	~CTestClass()
	{
	//	std::cout << "Destructed\n";
	}
	int A = -1;
};

void OptionalTestAllocation()
{
	CTestClass TestClass{ 1 };
	TOptional<CTestClass> First;
	First.SetValue(TestClass);
	TOptional<CTestClass> Second(First);
	ASSERT(Second.GetValue().A == 1, "Optional alloc error");
	Second.SetValue(2);
	ASSERT(Second.GetValue().A == 2, "Optional alloc error");
	First.Swap(Second);
	ASSERT(Second.GetValue().A == 1 && First.GetValue().A == 2, "Optional alloc error");
	First.Clear();
	const TOptional<CTestClass> Third(First);
	ASSERT(!Third.DoesValueExist(), "Optional alloc error");
}
