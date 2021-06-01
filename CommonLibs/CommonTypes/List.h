// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

#include <initializer_list>

#include "Iterators/DoublyLinked.h"
#include "./../CommonUtils/TypeOperations.h"

namespace Common
{
	/**
	 * @brief Container representing Doubly linked list.
	 * 
	 * @warning Work in progress. Sentinel node is not implemented yet, so
	 *			iterators do not work properly. There may be other bugs
	 * 
	 * Iterating over elements is slow, so you should use iterators that 
	 * store position. 
	 * Supports fixed-time add/removal of elements (and it is very fast).
	 * 
	 * TList WILL have methods like indexing for compatibility, but they 
	 * are supposed to be very slow because iteration is needed.
	 * 
	 * Exception policy: none (for now).
	 * 
	 * @note If the logical result of operation may not be determined,
	 *		 there are two methods: one that throws an exception ("Safe"
	 *		 prefix) and another one that does assertion in debug mode.
	 *		 Custom exception is COutOfRange, derived from CException.
	 * 
	 * @todo Unify methods with vector, add safe and auto methods
	 * 
	 * @todo Better iterator support
	 * 
	 * @todo Support numeric positions and indexing
	 * 
	 * @todo Implement SFINAE to support types without nonparam ctor
	 *		 and types without overloaded == operator
	 * 
	 * @bug Safe iterators do not track element removal
	*/
	template <typename T>
	class TList
	{

	private:

		class Node;

	public:

		typedef T value_type;	// Follow convention

		/// Iterator. Implemented op-s: ++, +=, +, --, -=, -, ==, !=, =.
		typedef Iterators::TDoublyLinkedIterator<Node*, T&> CIterator;
		/// Version of CIterator for const values.
		typedef Iterators::TDoublyLinkedIterator<const Node*, const T&> CConstIterator;
		/// Reverse iterator. Increment is actually decrement, etc.
		typedef Iterators::TReverseDoublyLinkedIterator<Node*, T&> CReverseIterator;
		/// Version of TReverseIterator for const values.
		typedef Iterators::TReverseDoublyLinkedIterator<const Node*, const T&>
			CConstReverseIterator;
		/// Iterator that does bounds checking and throws OutOfRange().
		typedef Iterators::TSafeDoublyLinkedIterator<Node*, T&> CSafeIterator;
		/// Version of TSafeIterator for const values.
		typedef Iterators::TSafeDoublyLinkedIterator<const Node*, const T&>
			CSafeConstIterator;
		/// Reverse iterator that can throw OutOfRange().
		typedef Iterators::TSafeReverseDoublyLinkedIterator<Node*, T&>
			CSafeReverseIterator;
		/// Version of TSafeReverseIterator for const values.
		typedef Iterators::TSafeReverseDoublyLinkedIterator
			<const Node*, const T&>CSafeConstReverseIterator;


		/// Creates empty list 
		TList() = default;

		////////////// TODO: add variant based on default ctor

		/**
		 * @brief List with pre-created elements
		 * @param Size Number of nodes to create
		 * @param DefaultValue Value to initialize with
		*/
		explicit TList(size_t Size, const T& DefaultValue = {});

		/**
		 * @brief Modern C++ initialization syntax: name = {...}.
		 * @param ValuesList Initializer list
		*/
		TList(const std::initializer_list<T>& ValuesList);

		/**
		 * @brief Constructor to get values from another container.
		 * @tparam IteratorType Iterator that implements ++, != and *
		 * @param Begin Iterator referring to the beginning of container
		 * @param End Iterator referring to the end of container
		*/
		template <typename IteratorType>
		TList(IteratorType Begin, IteratorType End,
			// resolving template constructors conflict (SFINAE)
			typename std::enable_if<!std::is_integral<
			IteratorType>::value >::type* = 0);

		/**
		 * @brief Initialize by copying another TList.
		 * @param Other list to copy
		*/
		TList(const TList<T>& Other);

		/**
		 * @brief Move constructor.
		 * @param Other Temporary object to get data from
		*/
		TList(TList<T>&& Other) noexcept;

		~TList();


		/**
		 * @brief Allows to copy values from another container.
		 * @tparam IteratorType Iterator that implements ++, != and *
		 * @param Begin Iterator referring to the beginning of container
		 * @param End Iterator referring to the end of container
		*/
		template <typename IteratorType>
		void Assign(IteratorType Begin, IteratorType End);

		TList<T>& operator = (const std::initializer_list<T>& ValuesList);

		TList<T>& operator = (const TList<T>& Other);

		TList<T>& operator = (TList<T>&& Other) noexcept;


		/**
		 * @brief Adds one element to the end of list.
		 * @param Value Element to add
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
		 * @brief Adds one element to the beginning of the list.
		 * @param Value Element to add
		*/
		void Unshift(const T& Value);

		/**
		 * @brief Adds multiple elements to the beginning via iterators.
		 * @tparam IteratorType Iterator with implemented ++, != and *
		 * @param Begin Iterator referring to the first element
		 * @param End Iterator referring to the element after last one
		*/
		template <typename IteratorType>
		void Unshift(IteratorType Begin, IteratorType End);

		/**
		 * @brief Inserts one element to the specified position
		 * @param Position Iterator pointing to insertion place
		 * @param Value Value to insert
		 * @note Position must be iterator to a valid node
		*/
		template <typename IteratorType>
		void Insert(IteratorType Position, const T& Value);

		/**
		 * @brief Adds multiple elements to the end via iterators.
		 * @tparam IteratorType1 Must be an iterator pointing to a valid node
		 * @tparam IteratorType2 Iterator with implemented ++, != and *
		 * @param Position Place where to start insertion from
		 * @param Begin Iterator referring to the first element
		 * @param End Iterator referring to the element after last one
		 * @note Position must be iterator to a valid node
		*/
		template <typename IteratorType1, typename IteratorType2>
		void Insert(IteratorType1 Position, IteratorType2 Begin,
			IteratorType2 End);

		/**
		 * @brief Removes one element from the end of list.
		 * @note List must not be empty.
		*/
		void Pop() noexcept;

		/**
		 * @brief Removes one element from the beginning of list.
		 * @note List must not be empty.
		*/
		void Shift() noexcept;

		template <typename IteratorType>
		void Erase(IteratorType Position) noexcept;


		/**
		 * @brief Swaps two lists internally without deep copy.
		 * @param Other Object to swap resources with
		*/
		void Swap(TList<T>& Other) noexcept;

		/**
		 * @brief Removes all elements from the list.
		 * @param ShrinkBehavior Optional. Describes how memory is freed
		*/
		void Clear() noexcept;


		/**
		 * @brief Size is number of elements that you can use
		 * @return Size of list
		*/
		size_t GetSize() const noexcept;

		/**
		 * @brief Simple check if size of this list equals 0.
		 * @return True if empty, false if not
		*/
		bool IsEmpty() const noexcept;


		/**
		 * @brief Provides access to the first element.
		 * @return Reference to the first element
		 * @note List must not be empty.
		*/
		T& Front();

		/**
		 * @brief Provides access to the last element.
		 * @return Reference to the last element
		 * @note List must not be empty.
		*/
		T& Back();

		/// Back() for const lists.
		const T& Back() const
		{
			return const_cast<TList<T>*>(this)->Back();
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

		size_t Size = 0;
		Node* Head = nullptr;
		Node* Tail = nullptr;

	};

	template<typename T>
	class TList<T>::Node
	{
	public:

		Node(T Value, Node* Previous = nullptr, Node* Next = nullptr)
			: Buffer(Value), Previous(Previous), Next(Next) {};

		T Buffer;
		Node* Next;
		Node* Previous;

	};

}

#include "Private/List/List.tpp"
#include "Private/List/Iterator.tpp"