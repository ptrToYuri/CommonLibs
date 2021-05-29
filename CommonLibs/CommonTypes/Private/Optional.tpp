// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

namespace Common
{

	template<typename T>
	TOptional<T>::TOptional(const T& Value)
		: bExists(true)
	{
		Construct(Value);
	}


	template<typename T>
	TOptional<T>::TOptional(const TOptional<T>& Other)
	{
		bExists = Other.bExists;
		if (bExists)
		{
			Construct(Other.Get());
		}
	}


	template<typename T>
	TOptional<T>::TOptional(TOptional<T>&& Other) noexcept
	{
		bExists = Other.bExists;
		if (bExists)
		{
			Construct(Move(Other.Get()));
		}
	}


	template<typename T>
	TOptional<T>::~TOptional()
	{
		DestructIfExists();
	}



	template<typename T>
	void TOptional<T>::SetValue(const T& Value)
	{
		DestructIfExists();
		Construct(Value);
		bExists = true;
	}


	template<typename T>
	void TOptional<T>::Clear() noexcept
	{
		DestructIfExists();
		bExists = false;
	}


	template<typename T>
	TOptional<T>& TOptional<T>::operator = (const T& Value)
	{
		SetValue(Value);
		return *this;
	}


	template<typename T>
	TOptional<T>& TOptional<T>::operator = (const TOptional<T>& Other)
	{
		DestructIfExists();
		if (Other.bExists)
		{
			Construct(Other.Get());
		}
		bExists = Other.bExists;
		return *this;
	}


	template<typename T>
	TOptional<T>& TOptional<T>::operator = (TOptional<T>&& Other) noexcept
	{
		DestructIfExists();
		if (Other.bExists)
		{
			Construct(Move(Other.Get()));
		}
		bExists = Other.bExists;
		return *this;
	}


	template<typename T>
	void TOptional<T>::Swap(TOptional<T>& Other)
	{
		::Swap(Other, *this);
	}



	template<typename T>
	bool TOptional<T>::operator == (const TOptional<T>& Other)
	{
		if (bExists != Other.bExists)
		{
			return false;
		}
		else if (!bExists)	// no need to call compare on internal type
		{					// bExists are surely equal there
			return true;
		}
		else
		{
			return Get() == Other.Get();
		}
	}



	template<typename T>
	bool TOptional<T>::DoesValueExist() const noexcept
	{
		return bExists;
	}


	template<typename T>
	const T& TOptional<T>::GetValue() const
	{
		if (!bExists)
		{
			throw CDoesNotExist("Optional does not contain value");
		}
		return Get();
	}


	template<typename T>
	const T& TOptional<T>::GetValueOr(const T& OtherVariant) const noexcept
	{
		if (bExists)
		{
			return Get();
		}
		else
		{
			return OtherVariant;
		}
	}



	template <typename T>
	void TOptional<T>::Construct(const T& Value)
	{
		new (Buffer) T(Value);
	}


	template <typename T>
	void TOptional<T>::DestructIfExists() noexcept
	{
		if (bExists)
		{
			reinterpret_cast<T*>(Buffer)->~T();
		}
	}


	template<typename T>
	inline const T& TOptional<T>::Get() const noexcept
	{
		return *reinterpret_cast<const T*>(Buffer);
	}

}