#pragma once

namespace Common
{

	/// Represents a pair of objects
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
			this.First = First;
			this->Second = Second;
		}


		/// First value in pair.
		T1 First = T1();
		/// Second value in pair.
		T2 Second = T2();

	};

}