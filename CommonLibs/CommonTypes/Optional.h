// Yuri Zamyatin, 2020-2021. This file is part of CommonLibs

#pragma once

// Suppress uninitialized allocated memory warning
#pragma warning(push)
#pragma warning(disable: 26495)

#include <new>	// placement new language feature

#include "Exception.h"
#include "./../CommonUtils/TypeOperations.h"	// Move, Swap

namespace Common
{

	/// Represents object that may not exist.
	template <typename T>
	class TOptional
	{

	public:

		/// No object by default.
		TOptional() = default;

		/**
		 * @brief Initialize optional with existing value (copy).
		 * @param Value Object to create copy from
		*/
		TOptional(const T& Value);

		/**
		 * @brief Initialize by copying another TOptional.
		 * @param Other Optional to create copy from
		*/
		TOptional(const TOptional<T>& Other);

		/**
		 * @brief Move constructor.
		 * @param Other Temporary object to get data from
		*/
		TOptional(TOptional<T>&& Other) noexcept;

		~TOptional();


		/**
		 * @brief Set value to optional (copy).
		 * @param Value Object to create copy from
		*/
		void SetValue(const T& Value);

		/**
		 * @brief Remove value from optional.
		 * @note This will call destructor on internal object.
		*/
		void Clear() noexcept;

		/**
		 * @brief Assign value to the optional (copy).
		 * @param Value Object to create copy from
		 * @return Reference to this optional
		*/
		TOptional<T>& operator = (const T& Value);

		/**
		 * @brief Assign from another optional (copy).
		 * @param Other Optional to create copy from
		 * @return Reference to this optional
		*/
		TOptional<T>& operator = (const TOptional<T>& Other);

		/**
		 * @brief Move assignment.
		 * @param Other Temporary object to get data from
		 * @return Reference to this optional
		*/
		TOptional<T>& operator = (TOptional<T>&& Other) noexcept;

		/**
		 * @brief Swaps two optionals without reconstructing values.
		 * @param Other Object to swap resources with
		*/
		void Swap(TOptional<T>& Other);


		/**
		 * @brief Checks if two optionals contain the same values.
		 * @param Other Other optional to compare
		 * @return True if sizes and values are equal, false otherwise
		 * @note Containing element must implement == operator.
		*/
		bool operator == (const TOptional<T>& Other);


		/**
		 * @brief Check if optional contains value.
		 * @return True if value exists, false otherwise
		*/
		bool DoesValueExist() const noexcept;

		/**
		 * @brief Gets value if it exists or throws an exception.
		 * @return Optional's value
		*/
		const T& GetValue() const;

		/**
		 * @brief Get value or passed value (if not possible).
		 * @param OtherVariant Returned if optional is empty
		 * @return Optional internal value or provided value
		*/
		const T& GetValueOr(const T& OtherVariant) const noexcept;

	private:

		alignas(T) unsigned char Buffer[sizeof(T)];
		bool bExists = false;
		
		// low level memory management
		inline void Construct(const T& Value);
		inline void DestructIfExists() noexcept;
		inline const T& Get() const noexcept;

	};

}

#include "Private/Optional.tpp"

#pragma warning(pop)