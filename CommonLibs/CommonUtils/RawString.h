#pragma once

namespace Common
{

	/**
	 * @brief Calculates length of the C-string.
	 * @param NullTermString Char array that ends with '\0'
	 * @return Number of actual letters in string ('\0' is
	 *		   not counted)
	*/
	size_t GetRawStringLength(const char* const NullTermString);

	/**
	 * @brief Calculates length of the C-string. Stops when null
	 *		  character is reached or MaxLength elements have
	 *		  been counted. Useful with malformatted input
	 * @param NullTermString Char array that ends with '\0'
	 *		  (or not, if you rely on MaxLength and buffer size)
	 * @param MaxLength Max amount of characters to count; does
	 *		  not include the trailing '\0'
	 * @return Number of actual letters in string ('\0' is
	 *		   not counted)
	*/
	size_t GetRawStringLength(const char* const NullTermString,
		size_t MaxLength);


	/**
	 * @brief Does the copy of C-style string (ended with '\0').
	 * @param NullTermStringFrom Source: char array that ends
	 *		  with '\0'
	 * @param NullTermStringTo Destination: Char array that is
	 *		  large enough to receive copied elements. May not
	 *		  end with '\0'
	*/
	void CopyRawString(const char* const NullTermStringFrom,
		char* const RawStringTo);

	/**
	 * @brief Does the copy of C-style string. Stops when null
	 *		  character is reached or MaxLength elements have
	 *		  been copied. Useful when string should be cut.
	 * @param NullTermStringFrom Source: char array that ends
	 *		  with '\0' (or not, if you rely on MaxLength and 
	 *		  buffer size)
	 * @param NullTermStringTo Destination: Char array that is
	 *		  large enough to receive copied elements. May not
	 *		  end with '\0'. After copying it gets '\0' anyway
	 * @param MaxLength Max amount of characters to copy; does
	 *		  not include trailing '\0'
	*/
	void CopyRawString(const char* const NullTermStringFrom,
		char* const RawStringTo, size_t MaxLength);


	/**
	 * @brief Checks whether two C-style strings are equal.
	 * @param NullTermString1 First null-terminated string
	 * @param NullTermString2 Second null-terminated string
	 * @return true if characters before '\0' are the same
	 *		   false otherwise
	*/
	bool AreRawStringsEqual(const char* const NullTermString1,
		const char* const NullTermString2);

	/**
	 * @brief Checks whether two C-style strings are equal.
	 *		  Use this to compare N first elements, even
	 *		  if strings are not null-terminated.
	 * @param NullTermString1 First string (whether null-
	 * 		  terminated or limited with MaxCompareLength)
	 * @param NullTermString2 Second string (whether null-
	 * 		  terminated or limited with MaxCompareLength)
	 * @param MaxComparedLength Max amount of characters to
	 *		  compare; does not include trailing '\0'
	 * @return true if characters before '\0' are the same
	 *		   false otherwise
	*/
	bool AreRawStringsEqual(const char* const NullTermString1,
		const char* const NullTermString2, size_t MaxCompareLength);
}