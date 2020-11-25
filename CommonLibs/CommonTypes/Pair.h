// Yuri Zamyatin, 2020. This file is part of CommonLibs

#pragma once

namespace Common
{

	/**
	 * @brief Container that represents a pair of objects.
	 * @note If the pair will be default-constucted, inner types
	 *		 must also implement constructors with no params
	 *		 
	*/
	template <typename T1, typename T2>
	class TPair
	{

	public:

		/// Initialize pair with type default values.
		TPair() = default;

		/**
		 * @brief Creates a pair copying passed values.
		 * @param First First value in pair
		 * @param Second Second value in pair
		*/
		TPair(const T1& First, const T2& Second) noexcept
			: First(First), Second(Second) {};


		/**
		 * @brief Assigns two values to pair at once (copy).
		 * @param First First value in pair
		 * @param Second Second value in pair
		*/
		void MakePair(T1 First, T2 Second) noexcept
		{
			this->First = First;
			this->Second = Second;
		}


		/// First value in pair.
		T1 First = T1{};
		/// Second value in pair.
		T2 Second = T2{};

	};



	template <typename T1, typename T2>
	inline TPair<T1, T2> MakePair(const T1& First,
		const T2& Second) noexcept
	{
		return TPair<T1, T2>(First, Second);
	}

}