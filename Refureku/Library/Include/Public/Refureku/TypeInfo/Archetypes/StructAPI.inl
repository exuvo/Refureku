/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

template <typename ReturnType, typename... ArgTypes>
ReturnType* StructAPI::makeInstance(ArgTypes&&... args) const
{
	static_assert(!std::is_pointer_v<ReturnType> && !std::is_reference_v<ReturnType>);

	if constexpr (sizeof...(args) == 0u)
	{
		//No arguments, use default instantiator
		return reinterpret_cast<ReturnType*>(makeInstanceFromDefaultInstantiator());
	}
	else
	{
		StaticMethodAPI const* instantiator;

		for (std::size_t i = 0u; i < getInstantiatorsCount(); i++)
		{
			instantiator = getInstantiatorAt(i);

			if (instantiator->hasSameParameters<ArgTypes...>())
			{
				//Custom instantiators are guaranteed to return void*
				return reinterpret_cast<ReturnType*>(instantiator->invoke<void*>(std::forward<ArgTypes>(args)...));
			}
		}

		return nullptr;
	}
}

template <typename T>
void* internal::defaultInstantiator()
{
	if constexpr (std::is_default_constructible_v<T>)
	{
		return new T();
	}
	else
	{
		return nullptr;
	}
}