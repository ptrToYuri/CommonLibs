// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

namespace Common 
{

	template<typename T>
	T* TVector<T>::Allocate(size_t NewSize)
	{

	}


	template<typename T>
	void TVector<T>::Deallocate(T* Buffer) noexcept
	{
		delete[] Buffer;
	}


	template<typename T>
	void TVector<T>::Construct(size_t Index, T* Buffer, T& value)
	{

	}


	template<typename T>
	void TVector<T>::Destruct(size_t Index, T* Buffer) noexcept
	{

	}


	template<typename T>
	void TVector<T>::DestructAll(size_t Size, T* Buffer) noexcept
	{

	}

}