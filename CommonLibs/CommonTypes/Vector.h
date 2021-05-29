// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

// Impossible to implement these libs because they're parts
// of the C++ compiler
#include <initializer_list>	// smart initialization syntax
#include <type_traits>

#include "Exception.h"
#include "Iterators/Block.h"
#include "../CommonUtils/Assert.h"
#include "./../CommonUtils/TypeOperations.h"	// Move, Swap
#include "./../CommonUtils/AdvancedIteration.h"	// Distance
#include "./../CommonUtils/BlockAllocation.h"	// Allocate, construct, ...

namespace Common
{

	/**
	 * @brief Container representing array that can change its size.
	 *
	 * Fast in accessing elements as offsets are used, but not very
	 * effective in terms of adding and removing elements. To minimize
	 * those drawbacks, there are memory reserving rules that you can
	 * manage.
	 * 
	 * Unlike std::vector, this has utilities to manage capacity
	 * outside. EReservedCapacityRule describes how extra capacity
	 * is reserved. This value is class member (get/set are possible).
	 * EShrinkBehavior can be passed to every function that is supposed
	 * to decrease TVector capacity, overriding EReservedCapacityRule
	 * in terms of removing elements in this specific case.
	 * 
	 * Exception policy: TVector stays in the previous state if
	 * construction fails. TVector is cleared if move construction of
	 * the underlying object failed. TVector is in the broken state
	 * if exception occured from TVector constructor, but destruction
	 * is handled correctly. Rethrows original exception.
	 * Capacity is not changed if CapacityRule is NOT NeverReserve.
	 *
	 * @note If the logical result of operation may not be determined,
	 *		 there are two methods: one that throws an exception ("Safe"
	 *		 prefix) and another one that does assertion in debug mode.
	 *		 Custom exception is COutOfRange, derived from CException.
	 *
	 * @note Underlying object must be copy and move constructible.
	 *
	 * @todo In case of construction errors, do not decrease capacity 
	 *		 unless CapacityRule is set to NeverReserve. 
	 *		 Capacity management is not consistent now, especially if
	 *		 move operation throws
	 * 
	 * @todo Implement SFINAE to support types without nonparam ctor
	 *		 and types without overloaded == operator
	 * 
	 * @bug Move may not be performed
	*/
	template <typename T>
	class TVector
	{

	public:

		typedef T value_type;	// Follow convention

		/// Iterator. Implemented op-s: ++, +=, +, --, -=, -, ==, !=, =.
		typedef Iterators::TBlockIterator<T*, T&> CIterator;
		/// Version of CIterator for const values.
		typedef Iterators::TBlockIterator<const T*, const T&> CConstIterator;
		/// Reverse iterator. Increment is actually decrement, etc.
		typedef Iterators::TReverseBlockIterator<T*, T&> CReverseIterator;
		/// Version of TReverseIterator for const values.
		typedef Iterators::TReverseBlockIterator<const T*, const T&>
			CConstReverseIterator;
		/// Iterator that does bounds checking and throws OutOfRange().
		typedef Iterators::TSafeBlockIterator<T*, T&, TVector<T>*> CSafeIterator;
		/// Version of TSafeIterator for const values.
		typedef Iterators::TSafeBlockIterator<const T*, const T&, const  TVector<T>*>
			CSafeConstIterator;
		/// Reverse iterator that can throw OutOfRange().
		typedef Iterators::TSafeReverseBlockIterator<T*, T&, TVector<T>*>
			CSafeReverseIterator;
		/// Version of TSafeReverseIterator for const values.
		typedef Iterators::TSafeReverseBlockIterator<const T*, const T&, const TVector<T>*>
			CSafeConstReverseIterator;


		/// Defines how extra Capacity is reserved
		enum class EReservedCapacityRule : uint8_t
		{
			/**
			 * [ADD] allocates (NewSize-1)*2 if capacity
			 * exceed and vector is not small enough <br>
			 * [DEL] allocates 2*Size if Capacity > 4 * Size
			 * and vector is not small enough
			*/
			Exponential,

			/**
			 * [ADD] allocates NewSize + 3 + 32 / sizeof(T) if
			 * capacity exceed <br>
			 * [DEL] allocates Size + 4 + 32 / sizeof(T) if size
			 * exceeds capacity by more than 2*(8 + 64 / sizeof(T))
			*/
			Linear,

			/**
			 * [ADD, DEL] memory is never reserved automatically;
			 * Decrease of vector size causes ShrinkToFit() immediately
			*/
			NeverReserve
		};

		/// Overrides CapacityRule for specific element removal case.
		enum class EShrinkBehavior
		{
			/**
			 * Force reallocation if possible.
			 * Same as calling ShrinkToFit() after each operation
			 * or setting EReservedCapacityRule to NeverReserve
			*/
			Require,

			/**
			 * Used as default value.
			 * Inherits behavior from EReservedCapacityRule.
			 * @see EReservedCapacityRule for more info about presets.
			*/
			Allow,

			/**
			 * Do not deallocate memory, even if EReservedCapacityRule
			 * prescribes that.
			*/
			Deny
		};


		/**
		 * @brief Creates empty vector with Capacity preset predefined.
		 * @param CapacityRule Optional. Describes how memory is reserved
		 * @see EReservedCapacityRule for more info about presets.
		*/
		TVector(EReservedCapacityRule CapacityRule
			= EReservedCapacityRule::Exponential) noexcept;

		////////////// TODO: add variant based on default ctor

		/**
		 * @brief Vector with pre-created elements.
		 * @param Size Number of elements to allocate
		 * @param DefaultValue Value to initialize with
		 * @param CapacityRule Optional. Describes how memory is reserved
		 * @see EReservedCapacityRule for more info about presets.
		*/
		TVector(size_t Size, const T& DefaultValue = {},
			EReservedCapacityRule CapacityRule
			= EReservedCapacityRule::Exponential);

		/**
		 * @brief Vector constructed from raw dynamic array (copy).
		 * @param Size Number of elements in original array
		 * @param Array Pointer to heap with C-style array
		 * @param CapacityRule Optional. Describes how memory is reserved
		 * @note Array[0] to Array[Size-1] must exist and have
		 *		 the same type as vector value_type.
		 * @see EReservedCapacityRule for more info about presets.
		*/
		TVector(size_t Size, const T* const Array,
			EReservedCapacityRule CapacityRule
			= EReservedCapacityRule::Exponential);

		/**
		 * @brief Modern C++ initialization syntax: name = {...}.
		 * @param ValuesList Initializer list
		*/
		TVector(const std::initializer_list<T>& ValuesList);

		/**
		 * @brief Constructor to get values from another container.
		 * @tparam IteratorType Iterator that implements ++, != and *
		 * @param Begin Iterator referring to the beginning of container
		 * @param End Iterator referring to the end of container
		 * @param CapacityRule Optional. Describes how memory is reserved
		 * @see EReservedCapacityRule for more info about presets.
		*/
		template <typename IteratorType>
		TVector(IteratorType Begin, IteratorType End,
			EReservedCapacityRule CapacityRule
			= EReservedCapacityRule::Exponential,
			// resolving template constructors conflict (SFINAE)
			typename std::enable_if<!std::is_integral<
			IteratorType>::value >::type* = 0);

		/**
		 * @brief Initialize by copying another TVector.
		 * @param Other vector to copy
		*/
		TVector(const TVector<T>& Other);

		/**
		 * @brief Move constructor.
		 * @param Other Temporary object to get data from
		*/
		TVector(TVector<T>&& Other) noexcept;

		~TVector();


		/**
		 * @brief Allows to copy values from another container.
		 * @tparam IteratorType Iterator that implements ++, != and *
		 * @param Begin Iterator referring to the beginning of container
		 * @param End Iterator referring to the end of container
		 * @param ShrinkBehavior Optional. Describes how memory is freed
		 * @see ShrinkBehavior for more info about patterns.
		*/
		template <typename IteratorType>
		void Assign(IteratorType Begin, IteratorType End,
			EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);

		/**
		 * @brief Allows assignment with = {...} style.
		 * @param ValuesList Initializer list
		 * @return Reference to this vector
		*/
		TVector<T>& operator = (const std::initializer_list<T>& ValuesList);

		/**
		 * @brief Makes a copy of another vector.
		 * @param Other vector
		 * @return Reference to this vector
		*/
		TVector<T>& operator = (const TVector<T>& Other);

		/**
		 * @brief Move assignment.
		 * @param Other Temporary object to get data from
		 * @return Reference to this vector
		*/
		TVector<T>& operator = (TVector<T>&& Other) noexcept;


		/**
		 * @brief Index operator that prodives access to the element.
		 * @param Index aka offset value
		 * @return Reference to the requested element
		 * @note Element with requested index must exist in vector.
		*/
		T& operator [] (size_t Index);

		/// Index operator for const vectors.
		const T& operator [] (size_t Index) const
		{
			return const_cast<TVector<T>*>(this)->operator[](Index);
		}

		/// [] with range check.
		T& SafeAt(size_t Index);

		/// SafeAt() for const vectors.
		const T& SafeAt(size_t Index) const
		{
			return const_cast<TVector<T>*>(this)->SafeAt(Index);
		}

		////////////// TODO: add variant based on default ctor

		/**
		 * @brief Provides access to the element. If range check fails,
		 *		   will auto fill vector up to Index with DefaultValue
		 * @param Index Element index
		 * @param DefaultValue Value to initialize added elements
		 * @return Reference to the requested element
		 * @see Use operator [] if you are sure that element exists
		*/
		T& AutoAt(size_t Index, const T& DefaultValue = {});


		/**
		 * @brief Provides access to the internal buffer
		 * @return Pointer to c-style heap array
		*/
		T* RawData() noexcept;

		/// RawData() for const vectors.
		const T* RawData() const noexcept
		{
			return const_cast<TVector<T>*>(this)->RawData();
		}


		/**
		 * @brief Checks if two vectors contain the same values by
		 *		  calling equality operator recursively
		 * @param Other Other vector to compare
		 * @return True if sizes and values are equal, false otherwise
		 * @note Containing elements must implement == operator
		*/
		bool operator == (const TVector<T>& Other) const noexcept;

		/// Opposite to operator ==.
		bool operator != (const TVector<T>& Other) const noexcept;

		/**
		 * @brief Concatenates vectors (push 1 with 2)
		 * @param Other Other vector to copy values from
		 * @return Reference to this vector
		*/
		TVector<T>& operator += (TVector<T>& Other);

		/**
		 * @brief Concatenates vectors (push 1 with 2)
		 * @param Other Other vector to copy values from
		 * @return New vector, containing elements from both vectors
		*/
		TVector<T> operator + (const TVector<T>& Other) const;


		/**
		 * @brief Adds one element to the end of vector.
		 * @param Value Element to add
		 * @see Call ShrinkToFit() to clear reserved memory,
		 *		Reserve() to increase its amount
		*/
		void Push(const T& Value);

		/**
		 * @brief Adds multiple elements to the end via iterators.
		 * @tparam IteratorType Iterator with implemented ++, != and *
		 * @param Begin Iterator referring to the first element
		 * @param End Iterator referring to the element after last one
		*/
		template <typename IteratorType>
		void Push(IteratorType Begin, IteratorType End);

		/**
		 * @brief Inserts one element to the specified position
		 * @param Position Index where to insert
		 * @param Value Value to insert
		 * @note Position must not exceed Size
		*/
		void Insert(size_t Position, const T& Value);

		/// Insert() with range check
		void SafeInsert(size_t Position, const T& Value);

		/**
		 * @brief Inserts element, extends vector if range check failed
		 * @param Position 
		 * @param Value	Value to insert
		 * @param DefaultValue Value to fill with if Position > Size
		 * @see Insert() if you are sure that Position <= Size
		*/
		void AutoInsert(size_t Position, const T& Value,
			const T& DefaultValue = {});

		/**
		 * @brief Inserts range of elements, starting at Position
		 * @tparam IteratorType Iterator with implemented ++, != and *
		 * @param Position Index of the first inserted element
		 * @param Begin Iterator referring to the first element
		 * @param End Iterator referring to the element after last one
		 * @note Position must not exceed Size
		*/
		template <typename IteratorType>
		void Insert(size_t Position, IteratorType Begin,
			IteratorType End);

		/// Insert() with range check
		template <typename IteratorType>
		void SafeInsert(size_t Position, IteratorType Begin,
			IteratorType End);

		////////////// TODO: add variant based on default ctor

		/**
		 * @brief Inserts range of elements, starting at Position.
		 *		  Extends vector if range check failed
		 * @tparam IteratorType Iterator with implemented ++, != and *
		 * @param Position Index of the first inserted element
		 * @param Begin Iterator referring to the first element
		 * @param End Iterator referring to the element after last one
		 * @param DefaultValue Value to fill with if Position > Size
		 * @see Insert() if you are sure that Position <= Size
		*/
		template <typename IteratorType>
		void AutoInsert(size_t Position, IteratorType Begin,
			IteratorType End, const T& DefaultValue = {});

		/**
		 * @brief Removes one element from the end of vector.
		 * @param ShrinkBehavior Optional. Describes how memory is freed
		 * @note Vector must not be empty.
		*/
		void Pop(EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);

		/// Pop() with range check
		void SafePop(EShrinkBehavior ShrinkBehavior
			= EShrinkBehavior::Allow);

		/// SafePop() that returns removed value
		T SafePopGet(EShrinkBehavior ShrinkBehavior
			= EShrinkBehavior::Allow);

		/**
		 * @brief Removes N elements from the end of vector.
		 * @param ElementsCount Number of elements to be removed
		 * @param ShrinkBehavior Optional. Describes how memory is freed
		 * @note If ElementsCount >= Size, clears vector
		*/
		void PopMultiple(size_t ElementsCount,
			EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);

		/**
		 * @brief Removes one element from the beginning of vector.
		 * @param ShrinkBehavior Optional. Describes how memory is freed
		 * @note Vector must not be empty.
		*/
		void Shift(EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);

		/// Shift() with range check
		void SafeShift(EShrinkBehavior ShrinkBehavior
			= EShrinkBehavior::Allow);

		/// SafeShift() that returns removed value
		T SafeShiftGet(EShrinkBehavior ShrinkBehavior
			= EShrinkBehavior::Allow);

		/**
		 * @brief Removes N elements from the beginning of vector.
		 * @param ElementsCount Number of elements to be removed
		 * @param ShrinkBehavior Optional. Describes how memory is freed
		 * @note If ElementsCount >= Size, clears vector
		*/
		void ShiftMultiple(size_t ElementsToShift,
			EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);

		/**
		 * @brief Removes element with specified position.
		 * @param Position Position of element to be removed
		 * @param ShrinkBehavior Optional. Describes how memory is freed
		 * @attention This method removes one element. To remove multiple,
		 *			  use EraseMultiple(). Your code with such a mistake
		 *			  may be compiled because of optional param.
		*/
		void Erase(size_t Position,
			EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);

		/// Erase() with range check
		void SafeErase(size_t Position,
			EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);

		/// SafeErase() that returns removed value
		T SafeEraseGet(size_t Position,
			EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);

		/**
		 * @brief Removes range of elements from vector
		 * @param PositionFrom Starting index for erase
		 * @param PositionTo End point for erase (after the last element)
		 * @param ShrinkBehavior Optional. Describes how memory is freed
		 * @note Ignores elements at unavailable positions.
		 * @attention This method removes multiple elements. To remove one,
		 *			  use Erase(). Your code with such a mistake may be
		 *			  compiled because of optional param.
		 *
		*/
		void EraseMultiple(size_t PositionFrom, size_t PositionTo,
			EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);


		/**
		 * @brief Allocates elements internally for the future use.
		 * @param NewCapacity If greater than size, will update
		 *		  internal capacity
		 * @see Call ShrinkToFit() to clear reserved memory.
		 * @attention Upon elements removal, vector can be shrinked
		 *			  if CapacityRule and ShrinkBehavior allow that
		*/
		void Reserve(size_t NewCapacity);

		/**
		 * @brief Changes size of vector
		 * @param NewSize New size of vector. If NewSize < Size, deletes
		 *		  last elements. Otherwise, creates new with passed value.
		 * @param DefaultValue Value to initialize added elements
		 * @param ShrinkBehavior Optional. Describes how memory is freed
		*/
		void Resize(size_t NewSize, const T& DefaultValue = {},
			EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);

		/**
		 * @brief Swaps two vectors internally without deep copy.
		 * @param Other Object to swap resources with
		*/
		void Swap(TVector<T>& Other) noexcept;

		/**
		 * @brief Clears memory that was reserved for future use.
		 * @see Methods that can reserve memory: Push(), Reserve(), etc.
		*/
		void ShrinkToFit();

		/**
		 * @brief Removes all elements from the vector.
		 * @param ShrinkBehavior Optional. Describes how memory is freed
		*/
		void Clear(EShrinkBehavior ShrinkBehavior = EShrinkBehavior::Allow);


		/**
		 * @brief Size is number of elements that you can use
		 * @return Size of vector
		*/
		size_t GetSize() const noexcept;

		/**
		 * @brief Capacity is Size + reserved space for the future use
		 * @return Capacity of vector
		*/
		size_t GetCapacity() const noexcept;

		/**
		 * @brief Simple check if size of this vector equals 0.
		 * @return True if empty, false if not
		*/
		bool IsEmpty() const noexcept;


		/**
		 * @brief Set enum value that will describe how re-allocation
		 *		  works when elements are added / removed from vertor.
		 *		  Reallocation on removal happens only if bool
		 *		  bAllowAutoShrink was passed with supported operation
		 * @param CapacityRule Preset value from EReservedCapacityRule
		 * @note If bAllowAutoShrink was passed with operation, size
		 *		 that you have manually reserved may also be deallocated
		*/
		void SetCapacityRule(EReservedCapacityRule CapacityRule) noexcept;

		/**
		 * @brief Returns Capacity rule that is currently applied.
		 *		  It affects how elements are allocated & destructed
		 * @return EReservedCapacityRule Current capacity rule
		 * @see EReservedCapacityRule for more info about presets.
		*/
		EReservedCapacityRule GetCapacityRule() const noexcept;


		/**
		 * @brief Provides access to the first element.
		 * @return Reference to the first element
		 * @note Vector must not be empty.
		*/
		T& Front();

		/// Front() for const vectors.
		const T& Front() const
		{
			return const_cast<TVector<T>*>(this)->Front();
		}

		/// Front() with range check.
		T& SafeFront();

		/// SafeFront() for const vectors.
		const T& SafeFront() const
		{
			return const_cast<TVector<T>*>(this)->SafeFront();
		}


		/**
		 * @brief Provides access to the last element.
		 * @return Reference to the last element
		 * @note Vector must not be empty.
		*/
		T& Back();

		/// Back() for const vectors.
		const T& Back() const
		{
			return const_cast<TVector<T>*>(this)->Back();
		}

		/// Back() with range check.
		T& SafeBack();

		/// SafeBack() for const vectors.
		const T& SafeBack() const
		{
			return const_cast<TVector<T>*>(this)->SafeBack();
		}


		/**
		 * @brief Iterator pointing to the first elem.
		 * @return CIterator iterator
		*/
		CIterator Begin();

		/**
		 * @brief Iterator pointing to the first elem (const).
		 * @return CConstIterator iterator
		*/
		CConstIterator ConstBegin() const;

		/**
		 * @brief Reverse iterator pointing to the first elem.
		 * @return CReverseIterator iterator
		*/
		CReverseIterator ReverseBegin();

		/**
		 * @brief Reverse iterator pointing to the first elem (const).
		 * @return CConstReverseIterator iterator
		*/
		CConstReverseIterator ConstReverseBegin() const;

		/**
		 * @brief Safe iterator pointing to the first elem.
		 * @return CSafeIterator iterator
		*/
		CSafeIterator SafeBegin();

		/**
		 * @brief Safe iterator pointing to the first elem (const).
		 * @return CSafeConstIterator iterator
		*/
		CSafeConstIterator SafeConstBegin() const;

		/**
		 * @brief Safe reverse iterator pointing to the first elem.
		 * @return CSafeReverseIterator iterator
		*/
		CSafeReverseIterator SafeReverseBegin();

		/**
		 * @brief Safe reverse iterator pointing to the first elem (const).
		 * @return CSafeConstReverseIterator
		*/
		CSafeConstReverseIterator SafeConstReverseBegin() const;

		/**
		 * @brief Iterator pointing to the elem after last.
		 * @return CIterator iterator
		*/
		CIterator End();

		/**
		 * @brief Iterator pointing to the elem after last (const).
		 * @return CConstIterator iterator
		*/
		CConstIterator ConstEnd() const;

		/**
		 * @brief Reverse iterator pointing to the elem after last.
		 * @return CReverseIterator iterator
		*/
		CReverseIterator ReverseEnd();

		/**
		 * @brief Reverse iterator pointing to the elem after last (const).
		 * @return CConstReverseIterator iterator
		*/
		CConstReverseIterator ConstReverseEnd() const;

		/**
		 * @brief Safe iterator pointing to the elem after last.
		 * @return CSafeIterator iterator
		*/
		CSafeIterator SafeEnd();

		/**
		 * @brief Safe iterator pointing to the elem after last (const).
		 * @return CSafeConstIterator iterator
		*/
		CSafeConstIterator SafeConstEnd() const;

		/**
		 * @brief Safe reverse iterator pointing to the elem after last.
		 * @return CSafeReverseIterator iterator
		*/
		CSafeReverseIterator SafeReverseEnd();

		/**
		 * @brief Safe reverse it. pointing to elem after last (const).
		 * @return CSafeConstReverseIterator
		*/
		CSafeConstReverseIterator SafeConstReverseEnd() const;


		/// Begin() alias (for compatibity)
		CIterator begin() { return Begin(); }
		/// ConstBegin() alias (overloaded, for compatibity)
		CConstIterator begin() const { return ConstBegin(); }
		/// ConstBegin() alias (for compatibity)
		CConstIterator cbegin() const { return ConstBegin(); }
		/// ReverseBegin() alias (for compatibity)
		CReverseIterator rbegin() { return ReverseBegin(); }
		/// ConstReverseBegin() alias (overloaded, for compatibity)
		CConstReverseIterator rbegin() const { return ConstReverseBegin(); }
		/// ConstReverseBegin() alias (for compatibity)
		CConstReverseIterator crbegin() const { return ConstReverseBegin(); }
		/// End() alias (for compatibity)
		CIterator end() { return End(); }
		/// ConstEnd() alias (overloaded, for compatibity)
		CConstIterator end() const { return ConstEnd(); }
		/// ConstEnd() alias (for compatibity)
		CConstIterator cend() const { return ConstEnd(); }
		/// ReverseEnd() alias (for compatibity)
		CReverseIterator rend() { return ReverseEnd(); }
		/// ConstReverseEnd() alias (overloaded, for compatibity)
		CConstReverseIterator rend() const { return ConstReverseEnd(); }
		/// ConstReverseEnd() alias (for compatibity)
		CConstReverseIterator crend() const { return ConstReverseEnd(); }

	private:

		// DO NOT CHANGE ORDER!!!
		size_t Size = 0;			// size assumed by user
		size_t Capacity = 0;		// actual allocated size
		T* Buffer = nullptr;		// storage
		EReservedCapacityRule CapacityRule =  // capacity management
			EReservedCapacityRule::Exponential;

		size_t CalcExtendedCapacity(size_t NewSize);
		void AutoShrinkIfNeeded(EShrinkBehavior ShrinkBehavior);


		friend CIterator;
		friend CConstIterator;
		friend CReverseIterator;
		friend CConstReverseIterator;
		friend CSafeIterator;
		friend CSafeConstIterator;
		friend CSafeReverseIterator;
		friend CSafeConstReverseIterator;

	};

}

#include "Private/Vector/Vector.tpp"
#include "Private/Vector/Iterator.tpp"