// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

#include "Pair.h"
#include "../CommonUtils/RawString.h"

namespace Common {

	/// Basic exception class. C-style message is required.
	class CException
	{

	public:

		/**
		 * @brief All exceptions must provide the message.
		 * @param Message Error description. Will be copied
		 *		  to an inner buffer
		 * @note If length of message > 47, first 47 symbols
		 *		 will be saved.
		*/
		CException(const char* Message) noexcept
		{
			CopyRawString(Message, this->Message, 47);
		};

		virtual ~CException() {}


		/**
		 * @brief Error message
		 * @return C-style string with error description
		*/
		virtual const char* GetMessage() const noexcept
		{
			return Message;
		}

	protected:

		char Message[48];
	};



	/// Represents "Out of Range" error. Can hold message, requested and expected indices.
	class COutOfRange : public CException
	{

	public:

		/**
		 * @brief Pass only message, if other properties cannot be specified.
		 * @param Message Description. Will be copied to an inner buffer
		 * @note Range will be set to empty [0: 0), requested index to 0.
		 * @note If length of message > 47, first 47 symbols will be saved.
		*/
		COutOfRange(const char* Message) noexcept
			: CException(Message) {};

		/**
		 * @brief Describes valid range and errored value. Contains message.
		 * @param Message Description. Will be copied to an inner buffer
		 * @param RequestedIndex Errored index (out of range).
		 * @param ExpectedIndex Pair of Min and Max+1 indexes, that were
		 *		  available. Range: [First: Second)
		 * @note If length of message > 47, first 47 symbols will be saved.
		*/
		COutOfRange(const char* Message,
			int RequestedIndex,
			const TPair<size_t, size_t>& ExpectedIndex) noexcept
			: CException(Message), RequestedIndex(RequestedIndex),
			ExpectedIndex(ExpectedIndex) {};

		/**
		 * @brief Specifies valid range.
		 * @return Pair of Min and Max+1 indexes, that were
		 *		   available. Range: [First: Second)
		 * @note Returns 0 Index with [0: 0) range if constructed
		 *		 only with message.
		*/
		const TPair<size_t, size_t>& GetExpectedRange() const noexcept
		{
			return ExpectedIndex;
		}

		/**
		 * @brief Index that caused this exception.
		 * @return Value of index, that is not in expected range
		 * @note Returns 0 Index with [0: 0) range if constructed
		 *		 only with message.
		*/
		int GetRequestedIndex() const noexcept
		{
			return RequestedIndex;
		}

	protected:

		const int RequestedIndex = 0;
		const TPair<size_t, size_t> ExpectedIndex = { 0,0 };

	};


	/// Represents allocation failed error (usually rethrown from new)
	class CBadAlloc : public CException
	{

	public:

		/**
		 * @brief Pass only message, if other properties cannot be specified.
		 * @param Message Description. Will be copied to the inner buffer
		 * @note If length of message > 47, first 47 symbols will be saved.
		*/
		CBadAlloc(const char* Message) noexcept
			: CException(Message) {};

		/**
		 * @brief Also specify requested alloc size
		 * @param Message Description. Will be copied to the inner buffer
		 * @param RequestedAllocSize Requested allocation size
		 * @return
		*/
		CBadAlloc(const char* Message, size_t RequestedAllocSize) noexcept
			: CException(Message), RequestedAllocSize(RequestedAllocSize) {};

		/**
		 * @brief Size in bytes that was intended to be allocated.
		 * @return Requested allocation size, that failed
		 * @note Returns 0 if constructed only with message.
		*/
		size_t GetRequestedAllocSize()
		{
			return RequestedAllocSize;
		}

	protected:

		const size_t RequestedAllocSize = 0;

	};



	/// Represents "Element does not exist" error. Stores message.
	class CDoesNotExist : public CException
	{

	public:

		/**
		 * @brief Pass only message, if other properties cannot be specified.
		 * @param Message Description. Will be copied to the inner buffer
		 * @note If length of message > 47, first 47 symbols will be saved.
		*/
		CDoesNotExist(const char* Message) noexcept
			: CException(Message) {};

	};

}