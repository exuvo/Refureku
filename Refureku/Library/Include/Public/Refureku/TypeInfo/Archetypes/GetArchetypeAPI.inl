/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

template <typename T>
ArchetypeAPI const* getArchetypeAPI() noexcept
{
	//Eliminate all unsupported types first
	if constexpr (std::is_function_v<T> || std::is_member_object_pointer_v<T>)
	{
		return nullptr;
	}
	else
	{
		//Get rid of cv-qualifiers, array, and references
		using RawType = std::decay_t<T>;

		if constexpr (std::is_pointer_v<RawType>)
		{
			return getArchetypeAPI<std::remove_pointer_t<RawType>>();
		}
		else if constexpr (std::is_class_v<RawType>)
		{
			if constexpr (std::is_const_v<T> || std::is_volatile_v<T> || std::is_reference_v<T>)
			{
				return getArchetypeAPI<RawType>();
			}
			else if constexpr (isCallable_staticGetArchetypeAPI<T, ArchetypeAPI const&()>::value)
			{
				return &T::staticGetArchetypeAPI();
			}
			else
			{
				return nullptr;
			}
		}
		else if constexpr (std::is_enum_v<RawType>)
		{
			return getEnumAPI<RawType>();
		}
		else
		{
			return nullptr;
		}
	}
}

template <template <typename...> typename T>
ArchetypeAPI const* getArchetypeAPI() noexcept
{
	return nullptr;
}

template <template <auto...> typename T>
ArchetypeAPI const* getArchetypeAPI() noexcept
{
	return nullptr;
}

template <template <template <typename...> typename...> typename T>
ArchetypeAPI const* getArchetypeAPI() noexcept
{
	return nullptr;
}

template <template <typename, auto> typename T>
ArchetypeAPI const* getArchetypeAPI() noexcept
{
	return nullptr;
}