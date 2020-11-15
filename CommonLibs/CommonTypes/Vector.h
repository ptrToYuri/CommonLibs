#pragma once

// Impossible to implement these libs because they're parts
// of the C++ compiler
#include <initializer_list>	// smart initialization syntax
#include <type_traits>	// tweaking the priority of overloaded methods
#include <new>	// rethrow std::bad_alloc

#include "Exception.h"
#include "../CommonUtils/Assert.h"
#include "../CommonUtils/AdvancedIteration.h"

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
	 * @note If the logical result of operation may not be determined,
	 *		 there are two methods: one that throws an exception ("Safe"
	 *		 prefix) and another one that does assertion in debug mode.
	 *		 Custom exception is COutOfRange, derived from CException
	 * 
	 * @note May throw std::bad_alloc if allocation fails.
	 *		 Allocation failure in assignment operations will cause the 
	 *		 vector to be cleared (as you intended to remove old elements
	 *		 anyway). If allocation fails in insertions, resize, push/pop,
	 *		 etc. - vector will stay in the previous state.
	*/
	template <typename T>
	class TVector
	{

	private:

		// PointerType was introduced to avoid double implementation
		// of const- and non-const iterators. Types are defined later
		template <typename PointerType, typename ReferenceType>
		class TIterator;
		template <typename PointerType, typename ReferenceType>
		class TReverseIterator;
		template <typename PointerType, typename ReferenceType>
		class TSafeIterator;
		template <typename PointerType, typename ReferenceType>
		class TSafeReverseIterator;

	public:

		/// Iterator. Implemented op-s: ++, +=, +, --, -=, -, ==, !=, =.
		typedef TIterator<T*, T&> CIterator;

		/// Version of CIterator for const values.
		typedef TIterator<const T*, const T&> CConstIterator;

		/// Reverse iterator. Increment is actually decrement, etc.
		typedef TReverseIterator<T*, T&> CReverseIterator;

		/// Version of TReverseIterator for const values.
		typedef TReverseIterator<const T*, const T&> CConstReverseIterator;

		/// Iterator that does bounds checking and throws OutOfRange().
		typedef TSafeIterator<T*, T&> CSafeIterator;

		/// Version of TSafeIterator for const values.
		typedef TSafeIterator<const T*, const T&> CSafeConstIterator;

		/// Reverse iterator that can throw OutOfRange().
		typedef TSafeReverseIterator<T*, T&> CSafeReverseIterator;

		/// Version of TSafeReverseIterator for const values.
		typedef TSafeReverseIterator<const T*, const T&>
			CSafeConstReverseIterator;


		/// Used to define how methods deal with Capacity.
		enum class EReservedCapacityRule : uint8_t
		{
			Exponential,
			Linear,
			NeverReserve
		};


		/// Empty vector with no heap allocation.
		TVector() = default;

		/**
		 * @brief Creates empty vector with Capacity preset predefined.
		 * @param CapacityRule Describes how memory will be reserved
		 * @see SetCapacityRule() for more info about presets.
		*/
		TVector(EReservedCapacityRule CapacityRule) noexcept;

		/**
		 * @brief Vector with pre-allocated elements
		 * @param Size Number of elements to allocate
		 * @param DefaultValue Optional. Value to initialize with
		 * @see Use Reserve() on empty vector to avoid initialization.
		*/
		explicit TVector(size_t Size, const T& DefaultValue = {});

		/**
		 * @brief Vector constructed from raw dynamic array (copy).
		 * @param Size Number of elements in original array
		 * @param Array Pointer to heap with C-style array
		 * @note Array[0] to Array[Size-1] must exist and have
		 *		 the same type as vector.
		 * @note Raw array is not changed.
		*/
		TVector(size_t Size, const T* const Array);

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
		*/
		template <typename IteratorType>
		TVector(IteratorType Begin, IteratorType End,
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
		*/
		template <typename IteratorType>
		void Assign(IteratorType Begin, IteratorType End,
			bool bAllowAutoShrink = false);

		/**
		 * @brief Allows assignment with = {...} style.
		 * @param ValuesList Initializer list
		 * @return Reference to this vector
		*/
		TVector<T>& operator = (const std::initializer_list<T>& ValuesList);

		/**
		 * @brief Assingment operator makes a copy of another vector.
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
			{ return const_cast<TVector<T>*>(this)->operator[](Index); }

		/// [] with range check.
		T& SafeAt(size_t Index);

		// SafeAt() for const vectors.
		const T& SafeAt(size_t Index) const
			{ return const_cast<TVector<T>*>(this)->SafeAt(Index); }

		/**
		 * @brief If element does not exist, this will resize vector and
		 *		  fill newly created elements with provided value
		 * @param Index aka offset value
		 * @param DefaultValue Value to initialize added elements
		 * @return Reference to the requested element
		 * @see Use operator [] if you are sure, that element exists
		*/
		T& AutoAt(size_t Index, const T& DefaultValue = {});


		/**
		 * @brief Provides access to the internal buffer
		 * @return Pointer to c-style heap array
		*/
		T* RawData() noexcept;

		/// RawData() for const vectors.
		const T* RawData() const noexcept
			{ return const_cast<TVector<T>*>(this)->RawData(); }


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
		 * @brief Adds one element to the end of vector.
		 * @param Value Element to add
		 * @see Call ShrinkToFit() to clear reserved memory,
		 *		Reserve() to increase it
		*/
		void Push(const T& Value);

		/**
		 * @brief Adds elements to the end, uses iterators
		 * @tparam IteratorType Iterator with implemented ++, != and *
		 * @param Begin Iterator referring to the first element
		 * @param End Iterator referring to the element after last one
		*/
		template <typename IteratorType>
		void Push(IteratorType Begin, IteratorType End);

		/**
		 * @brief Inserts one elements to the specified position
		 * @param Position Index where to insert
		 * @param Value Value to insert
		 * @param FillOnResizeWith If position is larger than vector size,
		 *		  elements that appear on resize will be initialized with
		 *		  this value
		 * @note If Position is greater than max index, vector is resized.
		*/
		void Insert(size_t Position, const T& Value = T {}, 
			const T& FillOnResizeWith = T{});

		/**
		 * @brief Inserts range of elements, starting at Position
		 * @tparam IteratorType Iterator with implemented ++, != and *
		 * @param Position Index of the first inserted element
		 * @param Begin Iterator referring to the first element
		 * @param End Iterator referring to the element after last one
		 * @param FillOnResizeWith If position is larger than vector size,
		 *		  elements that appear on resize will be initialized with
		 *		  this value
		 * @note If Position is greater than max index, vector is resized.
		*/
		template <typename IteratorType>
		void Insert(size_t Position, IteratorType Begin,
			IteratorType End, const T& FillOnResizeWith = T{},
			// another SFINAE trick resolving conflicts with implicit cast
			typename std::enable_if<!std::is_integral<
			IteratorType>::value >::type* = 0);
		
		/**
		 * @brief Removes one element from the end of vector.
		 * @param bAllowAutoShrink Optional. Enables auto shrink
		 *		  according to the capacity rule. See: SetCapacityRule()
		 * @return Removed element (by value)
		 * @note Vector must not be empty.
		*/
		T Pop(bool bAllowAutoShrink = false);

		/// Pop() with range check
		T SafePop(bool bAllowAutoShrink = false);

		/**
		 * @brief Removes N elements from the end of vector.
		 * @param ElementsCount Number of elements to be removed
		 * @param bAllowAutoShrink Optional. Enables auto shrink
		 *		  according to the capacity rule. See: SetCapacityRule()
		 * @note If ElementsCount >= Size, clears vector
		*/
		void PopMultiple(size_t ElementsCount, 
			bool bAllowAutoShrink = false);

		/**
		 * @brief Removes one element from the beginning of vector.
		 * @param bAllowAutoShrink Optional. Enables auto shrink
		 *		  according to the capacity rule. See: SetCapacityRule()
		 * @return Removed element (by value)
		*/
		T Shift(bool bAllowAutoShrink = false);

		/// Shift() with range check
		T SafeShift(bool bAllowAutoShrink = false);

		/**
		 * @brief Removes N elements from the beginning of vector.
		 * @param ElementsCount Number of elements to be removed
		 * @param bAllowAutoShrink Enables auto shrink
		 *		  according to the capacity rule. See: SetCapacityRule()
		 * @note If ElementsCount >= Size, clears vector
		*/
		void ShiftMultiple(size_t ElementsCount,
			bool bAllowAutoShrink = false);

		/**
		 * @brief Removes element with specified position.
		 * @param Position Position of element to be removed
		 * @param bAllowAutoShrink Enables auto shrink
		 *		  according to the capacity rule. See: SetCapacityRule()
		 * @note If Position is greater than max index, does nothing.
		 * @warning This method removes one element. To remove multiple,
		 *			use EraseMultiple(). Your code with such a mistake
		 *			will be compiled because of optional bool param.
		*/
		void Erase(size_t Position, bool bAllowAutoShrink = false);
		
		/**
		 * @brief Removes range of elements from vector
		 * @param PositionFrom Starting index for erase
		 * @param PositionTo End point for erase. Element with this index
		 *		  will also be removed
		 * @param bAllowAutoShrink Optional. Enables auto shrink
		 *		  according to the capacity rule. See: SetCapacityRule()
		 * @note Ignores elements at unavailable positions.
		 * @warning This method removes multiple elements. To remove one,
		 *			use Erase(). Your code with such a mistake will be
		 *			compiled because of optional bool param.
		 * 
		*/
		void EraseMultiple(size_t PositionFrom, size_t PositionTo,
			bool bAllowAutoShrink = false);


		/**
		 * @brief Allocates elements internally for the future use.
		 * @param NewCapacity If greater than size, will update
		 *		  internal capacity
		 * @see Call ShrinkToFit() to clear reserved memory.
		 * @note Passing bAllowAutoShrink to methods may cause shrink.
		*/
		void Reserve(size_t NewCapacity);

		/**
		 * @brief Changes size of vector
		 * @param NewSize New size of vector. If NewSize < Size, deletes
		 *		  last elements. Otherwise, creates new with passed value.
		 * @param DefaultValue Optional. Value to initialize added elements
		 * @param bAllowAutoShrink Optional. Enables auto shrink
		 *		  according to the capacity rule. See: SetCapacityRule()
		*/
		void Resize(size_t NewSize, const T& DefaultValue = {},
			bool bAllowAutoShrink = false);

		/**
		 * @brief Swaps two vectors internally without deep copy.
		 * @param Vector1 First vector
		 * @param Vector2 Second vector
		*/
		static void Swap(TVector<T>& Vector1, TVector<T>& Vector2) noexcept;

		/**
		 * @brief Clears memory that was reserved for future use.
		 * @see Methods that can reserve memory: push(), reserve(), etc.
		*/
		void ShrinkToFit();

		/// Removes all elements from the vector
		void Clear();


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
		 *
		 * EReservedCapacityRule::Exponential - [ADD] allocates 
		 * (NewSize-1)*2 if capacity exceed & vector is not small;
		 * [ERASE] allocates 2*Size if Capacity > 4 * Size and vector 
		 * is not small; <br>
		 * 
		 * EReservedCapacityRule::Linear - [ADD] allocates 
		 * NewSize + 3 + 32 / sizeof(T) if capacity exceed;
		 * [ERASE] allocates Size + 4 + 32 / sizeof(T) if size
		 * exceeds capacity by more than 2*(8 + 64 / sizeof(T)) <br>
		 * 
		 * 
		 * EReservedCapacityRule::NeverReserve - memory is never
		 * reserved automatically; if bAllowAutoShrink was passed
		 * into a method, then ShrinkToFit() is called immediately
		 *
		 * @param CapacityRule 
		 * @note If bAllowAutoShrink was passed with operation, size
		 *		 that you manually reserved may also be deallocated
		*/
		void SetCapacityRule(EReservedCapacityRule CapacityRule) noexcept;

		/**
		 * @brief Returns Capacity rule that is currently applied.
		 *		  It affects how elements are allocated & destructed
		 * @return EReservedCapacityRule 
		 * @see SetCapacityRule() for more info about presets.
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
			{ return const_cast<TVector<T>*>(this)->Front(); }

		/// Front() with range check.
		T& SafeFront();

		/// SafeFront() for const vectors.
		const T& SafeFront() const
			{ return const_cast<TVector<T>*>(this)->SafeFront(); }


		/**
		 * @brief Provides access to the last element.
		 * @return Reference to the last element
		 * @note Vector must not be empty.
		*/
		T& Back();

		/// Back() for const vectors.
		const T& Back() const
			{ return const_cast<TVector<T>*>(this)->Back(); }

		/// Back() with range check.
		T& SafeBack();

		/// SafeBack() for const vectors.
		const T& SafeBack() const
			{ return const_cast<TVector<T>*>(this)->SafeBack(); }

	
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

		inline size_t CalcExtendedCapacity(size_t NewSize);
		void Reallocate(size_t OldSize, size_t NewCapacity);
		inline T* AllocateOrResetAndThrow(size_t NewCapacity);
		void AutoShrinkIfNeeded();

		template <typename IteratorType>
		inline void CopyFromIterators(IteratorType Begin, 
			IteratorType End, T* Buffer);
		inline void CopyFromArray(size_t Size, const T* const Array1, T* Array2);

	};

}

#include "Private/VectorIterators.tpp"
#include "Private/Vector.tpp"