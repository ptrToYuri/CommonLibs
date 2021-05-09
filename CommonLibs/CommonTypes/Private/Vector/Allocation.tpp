// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

namespace Common
{

	template<typename T>
	inline T* TVector<T>::Allocate(size_t NewSize)
	{
		return reinterpret_cast<T*>(::operator new (sizeof(T) * NewSize));
	}


	template<typename T>
	inline void TVector<T>::Deallocate(T* Buffer) noexcept
	{
		::operator delete[](Buffer);
	}


	template<typename T>
	inline void TVector<T>::Construct(size_t Index, T* Buffer, T& value)
	{
		new (Buffer[Index]) T(value);
	}


	template<typename T>
	inline void TVector<T>::Destruct(size_t Index, T* Buffer) noexcept
	{
		Buffer[Index]->~T();
	}


	template<typename T>
	inline void TVector<T>::DestructAll(size_t Size, T* Buffer) noexcept
	{
		for (size_t i = 0; i < Size; ++i) {
			Destruct(i, Buffer);
		}
	}

}