// Yuri Zamyatin, 2020. This file is part of CommonLibs

#pragma once

#include "Exception.h"

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
		TOptional(const T& Value) noexcept
			: Value(T), bExists(true) {};


		/**
		 * @brief Set value to optional (copy).
		 * @param Value Object to create copy from
		*/
		void SetValue(const T& Value) noexcept
		{
			this->Value = Value;
			bExists = true;
		}

		/**
		 * @brief Set value to the optional (copy).
		 * @param Value Object to create copy from
		 * @return Reference to this optional
		*/
		TOptional<T>& operator = (const T& Value) noexcept
		{
			SetValue(Value);
		}

		/**
		 * @brief Check if optional has value.
		 * @return True if value exists
		*/
		bool ValueExists() const noexcept
		{
			return bExists;
		}

		/**
		 * @brief Gets value if it exists or throws an exception.
		 * @return Optional's value
		*/
		const T& GetValue() const
		{
			if (!bExists)
			{
				throw CDoesNotExist("Optional does not contain value");
			}
			return Value;
		}

		/**
		 * @brief Get value or passed value (if not possible)
		 * @param OtherVariant 
		 * @return Optional value or provided value 
		*/
		const T& GetValueOr(const T& OtherVariant) const noexcept
		{
			if (bExists)
			{
				return Value;
			}
			else
			{
				return OtherVariant;
			}
		}

	private:

		T Value = T();
		bool bExists = false;

	};

}
