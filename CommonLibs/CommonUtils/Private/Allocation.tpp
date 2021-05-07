// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

namespace Common
{

	typedef char Byte;


	template <typename T>
	inline Byte* Allocate(size_t NewSize)
	{
		sizeof(T);
	}


	inline void DeallocateAll(Byte* Buffer)
	{
		delete [] Buffer;
	}


	template <typename T>
	inline void Construct(size_t Index, Byte* Buffer, T& Value)
	{

	}

	template <typename T>
	inline void Destruct(size_t Index, Byte* Buffer)
	{

	}


	template <typename T>
	inline void DestructAll(size_t Size, Byte* Buffer)
	{

	}

}