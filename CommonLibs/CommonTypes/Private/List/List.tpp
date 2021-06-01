// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

namespace Common
{

	template<typename T>
	TList<T>::TList(const size_t Size, const T& DefaultValue)
		:Size(Size)
	{
		if (Size == 0)
		{
			return;
		}

		Head = new Node(DefaultValue);
		Node* Current = Head;
		for (int i = 1; i < Size; ++i)
		{
			Current->Next = new Node(DefaultValue, Current);
			Current = Current->Next;
		}
		Tail = Current;
	}


	template<typename T>
	TList<T>::TList(const std::initializer_list<T>& ValuesList)
		: TList(ValuesList.begin(), ValuesList.end()) {};


	template<typename T>
	template<typename IteratorType>
	TList<T>::TList(IteratorType Begin, IteratorType End,
		typename std::enable_if<!std::is_integral<
		IteratorType>::value>::type*)
	{
		if (Begin == End)
		{
			return;
		}

		Head = new Node(*Begin);
		++Begin;
		Size = 1;
		Node* Current = Head;
		for (auto it = Begin; it != End; ++it)
		{
			Current->Next = new Node(*it, Current);
			Current = Current->Next;
			++Size;
		}
		Tail = Current;
	}


	template<typename T>
	TList<T>::TList(const TList<T>& Other)
		:TList(Other.cbegin(), Other.cend())
	{
		if (Other.Size)
		{
			Push(Other.Back());	// hack - iterators are broken due to non circularity
		}						// todo: fix
	};


	template<typename T>
	TList<T>::TList(TList<T>&& Other) noexcept
	{
		Swap(Other);
	}


	template<typename T>
	TList<T>::~TList()
	{
		Clear();
	}



	template<typename T>
	template<typename IteratorType>
	void TList<T>::Assign(IteratorType Begin, IteratorType End)
	{
		if (Begin == End)
		{
			Clear();
			return;
		}

		size_t TempSize = 1;
		Node* TempHead = new Node(*Begin);
		++Begin;
		Node* Current = TempHead;
		for (auto it = Begin; it != End; ++it)
		{
			Current->Next = new Node(*it, Current);
			Current = Current->Next;
			++TempSize;
		}

		Clear();

		Swap(Head, TempHead);
		Size = TempSize;
		Tail = Current;
	}


	template<typename T>
	TList<T>& TList<T>::operator=(const std::initializer_list<T>& ValuesList)
	{
		Assign(ValuesList.begin(), ValuesList.end());
		return *this;
	}


	template<typename T>
	TList<T>& Common::TList<T>::operator=(const TList<T>& Other)
	{
		Assign(Other.cbegin(), Other.cend());
		return *this;
	}


	template<typename T>
	TList<T>& TList<T>::operator=(TList<T>&& Other) noexcept
	{
		Swap(Other);
		return *this;
	}



	template<typename T>
	void TList<T>::Push(const T& Value)
	{
		Node* Temp = new Node(Value, Tail, nullptr);
		if (Tail == nullptr)
		{
			Head = Temp;
		}
		else
		{
			Tail->Next = Temp;
		}
		Tail = Temp;
		++Size;
	}


	template<typename T>
	template<typename IteratorType>
	void TList<T>::Push(IteratorType Begin, IteratorType End)
	{
		if (Begin == End)
		{
			return;
		}

		if (Head == nullptr)
		{
			Push(*Begin);
			++Begin;
			++Size;
		}
		Node* Current = Tail;

		for (auto it = Begin; it != End; ++it)
		{
			Current->Next = new Node(*it, Current);
			Current = Current->Next;
			++Size;
		}
		Tail = Current;
	}


	template<typename T>
	void TList<T>::Unshift(const T& Value)
	{
		Node* Temp = new Node(Value, nullptr, Head);
		if (Head == nullptr)
		{
			Tail = Temp;
		}
		else
		{
			Head->Previous = Temp;
		}
		Head = Temp;
		++Size;
	}


	template<typename T>
	template<typename IteratorType>
	void TList<T>::Unshift(IteratorType Begin, IteratorType End)
	{
		if (Begin == End)
		{
			return;
		}

		if (Head == nullptr)
		{
			Unshift(*Begin);
			++Begin;
			++Size;
		}
		Node* Current = Head;

		for (auto it = Begin; it != End; ++it)
		{
			Current->Previous = new Node(*it, nullptr, Current);
			Current = Current->Next;
			++Size;
		}
		Head = Current;
	}

	template<typename T>
	template<typename IteratorType>
	void TList<T>::Insert(IteratorType Position, const T& Value)
	{
		if (Position == nullptr)
		{
			return;
		}
		if (Position.NodePointer->Previous == nullptr)
		{
			Unshift(Value);
			++Size;
		}
	}


	template<typename T>
	template<typename IteratorType1, typename IteratorType2>
	void Common::TList<T>::Insert(IteratorType1 Position, IteratorType2 Begin, IteratorType2 End)
	{
		if (Begin == End || Position == nullptr)
		{
			return;
		}

		if (Position.NodePointer->Previous == nullptr)
		{
			Unshift(*Begin);
			++Size;
			++Begin;
		}

		Node* Current = Position.NodePointer->Previous;
		Node* LinkTo = Position.NodePointer;

		for (auto it = Begin; it != End; ++it)
		{
			Current->Next = new Node(*it, Current);
			Current = Current->Next;
			++Size;
		}
		LinkTo->Previous = Current;
		Current->Next = LinkTo;
	}


	template<typename T>
	void TList<T>::Pop() noexcept
	{
		if (Head == nullptr)
		{
			return;
		}
		if (Head == Tail)
		{
			delete Tail;
			Head = nullptr;
			Tail = nullptr;
		}
		else
		{
			Node* Temp = Tail;
			Tail = Tail->Previous;
			Tail->Next = nullptr;
			delete Temp;
		}
		--Size;
	}


	template<typename T>
	void TList<T>::Shift() noexcept
	{
		if (Head == nullptr)
		{
			return;
		}
		if (Head == Tail)
		{
			delete Head;
			Head = nullptr;
			Tail = nullptr;
		}
		else
		{
			Node* Temp = Head;
			Head = Head->Next;
			Head->Previous = nullptr;
			delete Temp;
		}
		--Size;
	}


	template<typename T>
	template<typename IteratorType>
	void TList<T>::Erase(IteratorType Position) noexcept
	{
		if (Position->Previous == nullptr)
		{
			Shift();
		}
		else if (Position->Next == nullptr)
		{
			Pop();
		}
		else
		{
			Position->Previous = Position->Next;
			Position->Next = Position->Previous;
			delete Position;
		}
		--Size;
	}



	template<typename T>
	void TList<T>::Swap(TList<T>& Other) noexcept
	{
		Common::Swap(Other.Head, this->Head);
		Common::Swap(Other.Tail, this->Tail);
		Common::Swap(Other.Size, this->Size);
	}

	template<typename T>
	void TList<T>::Clear() noexcept
	{
		Node* Next = Head;
		Node* Current = nullptr;
		while (Next != nullptr) {
			Current = Next;
			Next = Current->Next;
			delete Current;
		}
		Size = 0;
	}

	template<typename T>
	size_t TList<T>::GetSize() const noexcept
	{
		return Size;
	}

	template<typename T>
	bool TList<T>::IsEmpty() const noexcept
	{
		return Size == 0;
	}

	template<typename T>
	T& TList<T>::Front()
	{
		return Head->Buffer;
	}

	template<typename T>
	T& TList<T>::Back()
	{
		return Tail->Buffer;
	}

}