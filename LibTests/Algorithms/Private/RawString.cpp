#include "../RawString.h"

using namespace Common;


void RawStringTestLength()
{
	ASSERT(GetRawStringLength("Hi!") == 3, "Old string length error");
	ASSERT(GetRawStringLength("") == 0, "Old string length error");
	ASSERT(GetRawStringLength("ABCDEF", 3) == 3, "Old string length error");
	ASSERT(GetRawStringLength("AB", 300) == 2, "Old string length error");
}


void RawStringTestEqual()
{
	char First[50] = "ABCDEF";
	char Second[1] = "";
	char Third[5] = "ABCD";
	ASSERT(AreRawStringsEqual("ABCDEF", First), "Old string length error");
	ASSERT(AreRawStringsEqual("", Second), "Old string length error");
	ASSERT(!AreRawStringsEqual("ABCDEF", Third), "Old string length error");
	ASSERT(!AreRawStringsEqual("AB", Third), "Old string length error");
	ASSERT(AreRawStringsEqual("ABCD", Third), "Old string length error");
	ASSERT(AreRawStringsEqual("ABCDEFG", "ABC", 2), "Old string length error");
	ASSERT(AreRawStringsEqual("", "ABC", 0), "Old string length error");
	ASSERT(!AreRawStringsEqual("AC", "ABC", 2), "Old string length error");
	ASSERT(!AreRawStringsEqual("A", "ABC", 2), "Old string length error");
}


void RawStringTestCopy()
{
	char First[10] = "ABCDEFG";
	char Second[10];
	CopyRawString(First, Second);
	ASSERT(Second[7] == '\0', "Old string copy error");
	ASSERT(AreRawStringsEqual(First, Second), "Old string copy error");

	CopyRawString("ABCKHDG", Second, 3);
	ASSERT(AreRawStringsEqual(Second, "ABC"), "Old string copy error");

	CopyRawString("ABCKHDG", Second, 0);
	ASSERT(AreRawStringsEqual(Second, ""), "Old string copy error");
}
