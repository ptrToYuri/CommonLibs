#pragma once

#include "Pair.h"

namespace Common {

	/// Basic exception class. C-style message is required
	class CException
	{

	public:

		/**
		 * @brief All exceptions must provide the message
		 * @param Message Error description
		*/
		CException(const char* const Message) noexcept
			: Message(Message) {};

		virtual ~CException() {}


		/**
		 * @brief Error message
		 * @return C-style string with error description
		*/
		virtual const char* GetMessage() noexcept
		{
			return Message;
		}

	protected:

		const char* const Message;

	};



	class COutOfRange : protected CException
	{

	public:

		/**
		 * @brief Pass only message, if other properties cannot be specified.
		 * @param Message Error description
		 * @note Range will be set to empty [0, 0), requested index to 0
		*/
		COutOfRange(const char* const Message)
			: CException(Message) {};

		/**
		 * @brief Describes valid range and errored value. Also contains message.
		 * @param Message Error description
		 * @param RequestedIndex Errored index (out of range).
		 * @param ExpectedIndex Pair of Min and Max+1 indexes, that were available.
		 *		  Range: [First: Second)
		*/
		COutOfRange(const char* const Message,
			size_t RequestedIndex, const TPair<size_t, size_t>& ExpectedIndex)
			: CException(Message), RequestedIndex(RequestedIndex),
			ExpectedIndex(ExpectedIndex) {};

		/**
		 * @brief Specifies valid range.
		 * @return Pair of Min and Max+1 indexes, that were 
		 *		   available. Range: [First: Second)
		 * @note Returns 0 Index with [0,0) range if constructed only
		 *		 with message.
		*/
		virtual const TPair<size_t, size_t>& GetExpectedRange() const
		{
			return ExpectedIndex;
		}

		/**
		 * @brief Index that caused this exception.
		 * @return Value of index, that is not in expected range
		 * @note Returns 0 Index with [0,0) range if constructed only
		 *		 with message.
		*/
		virtual size_t GetRequestedIndex() const
		{
			return RequestedIndex;
		}

	protected:

		size_t RequestedIndex = 0;
		TPair<size_t, size_t> ExpectedIndex = { 0,0 };

	};



	class CDoesNotExist : protected CException
	{

	public:

		/**
		 * @brief Pass only message, if other properties cannot be specified
		 * @param Message Error description
		*/
		CDoesNotExist(const char* const Message)
			: CException(Message) {};

	};

}