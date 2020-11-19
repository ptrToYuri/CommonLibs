// Yuri Zamyatin, 2020. This file is part of CommonLibs

#include "../RawString.h"

namespace Common
{

	size_t GetRawStringLength(const char* const NullTermString)
	{
		size_t Length = 0;
		while (NullTermString[Length] != '\0')
		{
			++Length;
		}
		return Length;
	}


	size_t GetRawStringLength(const char* const NullTermString,
		size_t MaxLength)
	{
		size_t Length = 0;
		while (NullTermString[Length] != '\0' && Length < MaxLength)
		{
			++Length;
		}
		return Length;
	}



	void CopyRawString(const char* const NullTermStringFrom,
		char* const NullTermStringTo)
	{
		size_t i = 0;
		while (NullTermStringFrom[i] != '\0')
		{
			NullTermStringTo[i] = NullTermStringFrom[i];
			++i;
		}
		NullTermStringTo[i] = '\0';
	}


	void CopyRawString(const char* const NullTermStringFrom,
		char* const NullTermStringTo, size_t MaxLength)
	{
		size_t i = 0;
		while (NullTermStringFrom[i] != '\0' && i < MaxLength)
		{
			NullTermStringTo[i] = NullTermStringFrom[i];
			++i;
		}
		NullTermStringTo[i] = '\0';
	}



	bool AreRawStringsEqual(const char* const NullTermString1,
		const char* const NullTermString2)
	{
		size_t i = 0;
		while (NullTermString1[i] == NullTermString2[i])
		{
			if (NullTermString1[i] == '\0' && NullTermString2[i] == '\0')
			{
				return true;
			}
			++i;
		}
		return false;
	}

	bool AreRawStringsEqual(const char* const NullTermString1,
		const char* const NullTermString2, size_t MaxCompareLength)
	{
		size_t i = 0;
		while (NullTermString1[i] == NullTermString2[i]
			&& i < MaxCompareLength - 1)
		{
			if (NullTermString1[i] == '\0' && NullTermString2[i] == '\0')
			{
				return true;
			}
			++i;
			if (NullTermString1[i] != NullTermString2[i])
			{
				return false;
			}
		}
		return true;
	}

}