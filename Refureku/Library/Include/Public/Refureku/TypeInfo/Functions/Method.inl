/**
*	Copyright (c) 2021-2022 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

////////////////// MemberFunctionSafeCallWrapper

template <typename ReturnType, typename... ArgTypes>
template <typename T, typename>
ReturnType Method::MemberFunctionSafeCallWrapper<ReturnType(ArgTypes...)>::invoke(ICallable const& method, T* caller, ArgTypes&&... args)
{
	//Some hacky implementation for MSVC to handle the fact that
	//pointer to member functions can have different sizes based on the class inheritance
	class NoInheritanceClass {};

#if (defined(_WIN32) || defined(_WIN64))

	//Define all cases that generate different pointer to member function sizes
	//class NoInheritanceClass {};	//No inheritance / Single inheritance -> x32: 4 / x64: 8
	class NoInheritanceClass2 {};
	class MultipleInheritanceClass : public NoInheritanceClass, public NoInheritanceClass2 {};	//Multiple inheritance -> x32: 8 / x64: 16
	class VirtualInheritanceClass : public virtual NoInheritanceClass {}; //Virtual inheritance -> x32: 16 / x64: 16

	switch (reinterpret_cast<MemberFunction<NoInheritanceClass, ReturnType(ArgTypes...)> const&>(method).getOriginalFunctionSize())
	{
		case (sizeof(PointerToMemberMethod<NoInheritanceClass>)):
			return reinterpret_cast<MemberFunction<NoInheritanceClass, ReturnType(ArgTypes...)> const&>(method).operator()(*reinterpret_cast<typename CopyConstness<T, NoInheritanceClass>::Type*>(caller), std::forward<ArgTypes>(args)...);

		case (sizeof(PointerToMemberMethod<MultipleInheritanceClass>)):
			return reinterpret_cast<MemberFunction<MultipleInheritanceClass, ReturnType(ArgTypes...)> const&>(method).operator()(*reinterpret_cast<typename CopyConstness<T, MultipleInheritanceClass>::Type*>(caller), std::forward<ArgTypes>(args)...);

#if !defined(_WIN64) //_WIN32
		case (sizeof(PointerToMemberMethod<VirtualInheritanceClass>)):
			return reinterpret_cast<MemberFunction<VirtualInheritanceClass, ReturnType(ArgTypes...)> const&>(method).operator()(*reinterpret_cast<typename CopyConstness<T, VirtualInheritanceClass>::Type*>(caller), std::forward<ArgTypes>(args)...);
#endif

		default:
			assert(false);

			//Fake a return to avoid warnings
			//Should never reach this point anyway
			return reinterpret_cast<ReturnType (*)()>(0)();
	}
#else
	//No trick required, pointer to member function size does not vary with class inheritance on GCC / Clang
	//Cast to a random dummy class just to fake a class member (because we can't have a MemberFunction<void, ...>).
	return reinterpret_cast<MemberFunction<NoInheritanceClass, ReturnType(ArgTypes...)> const&>(method).operator()(*reinterpret_cast<typename CopyConstness<T, NoInheritanceClass>::Type*>(caller), std::forward<ArgTypes>(args)...);
#endif
}

////////////////// Method

template <typename ReturnType, typename... ArgTypes>
ReturnType Method::internalInvoke(void* caller, ArgTypes&&... args) const
{
	return MemberFunctionSafeCallWrapper<ReturnType(ArgTypes...)>::invoke(*getInternalFunction(), caller, std::forward<ArgTypes>(args)...);
}

template <typename ReturnType, typename... ArgTypes>
ReturnType Method::internalInvoke(void const* caller, ArgTypes&&... args) const
{
	return MemberFunctionSafeCallWrapper<ReturnType(ArgTypes...)>::invoke(*getInternalFunction(), caller, std::forward<ArgTypes>(args)...);
}

template <typename CallerType>
CallerType* Method::adjustCallerAddress(CallerType& caller) const noexcept
{
	if constexpr (std::is_base_of_v<Object, CallerType>)
	{
		return adjustCallerAddress(caller, caller.getArchetype());
	}
	else
	{
		//Can't retrieve the dynamic archetype through a virtual getArchetype call, so use the caller static archetype.
		// !!! If a memory offset exists between the caller static and dynamic archetypes, the returned result is INCORRECT !!!
		return adjustCallerAddress(caller, *static_cast<rfk::Struct const*>(rfk::getArchetype<CallerType>()));
	}
}

template <typename CallerType>
CallerType* Method::adjustCallerAddress(CallerType& caller, Struct const& callerDynamicArchetype) const noexcept
{
	//No adjustment required if the method is not virtual
	//The adjustment is required for virtual methods to point to the correct vtable
	if (isVirtual())
	{
		rfk::Struct const* callerStaticArchetype = static_cast<rfk::Struct const*>(rfk::getArchetype<CallerType>());

		if (callerStaticArchetype != nullptr)
		{
			CallerType* adjustedCallerPointer = rfk::dynamicCast<CallerType>(&caller,
																			 *callerStaticArchetype,
																			 callerDynamicArchetype,
																			 *static_cast<rfk::Struct const*>(getOuterEntity())
																			);

			if (adjustedCallerPointer != nullptr)
			{
				return adjustedCallerPointer;
			}
			else
			{
				//adjustCallerAddress doesn't fail if the dynamicCast fails, and the original caller is returned as provided
				//The invoke call using this caller is UB.
			}
		}
	}

	return &caller;
}

template <typename CallerType>
CallerType* Method::checkedAdjustCallerAddress(CallerType& caller) const
{
	static_assert(std::is_base_of_v<Object, CallerType>,
				  "[Refureku] To perform all the safety checks, the caller must implement the getArchetype() method inherited from rfk::Object.");

	return checkedAdjustCallerAddress(caller, caller.getArchetype());
}

template <typename CallerType>
CallerType* Method::checkedAdjustCallerAddress(CallerType& caller, Struct const& callerDynamicArchetype) const
{
	//No adjustment required if the method is not virtual
	//The adjustment is required for virtual methods to point to the correct vtable
	if (isVirtual())
	{
		rfk::Struct const* callerStaticArchetype = static_cast<rfk::Struct const*>(rfk::getArchetype<CallerType>());

		//Should not be able to be nullptr since the caller inherits from rfk::Object
		assert(callerStaticArchetype != nullptr);

		CallerType* adjustedCallerPointer = rfk::dynamicCast<CallerType>(&caller,
																		 *callerStaticArchetype,
																		 callerDynamicArchetype,
																		 *static_cast<rfk::Struct const*>(getOuterEntity())
																		 );

		if (adjustedCallerPointer != nullptr)
		{
			return adjustedCallerPointer;
		}
		else
		{
			//At this point, the cast can only fail if caller is not in the same inheritance hierarchy as the method's owner struct
			throwInvalidCallerException();
		}
	}

	return &caller;
}

template <typename ReturnType, typename CallerType, typename... ArgTypes, typename>
ReturnType Method::invoke(CallerType& caller, ArgTypes&&... args) const
{
	return internalInvoke<ReturnType, ArgTypes...>(adjustCallerAddress(caller), std::forward<ArgTypes>(args)...);
}

template <typename ReturnType, typename CallerType, typename... ArgTypes>
ReturnType Method::invoke(CallerType const& caller, ArgTypes&&... args) const
{
	if (!isConst())
	{
		throwConstViolationException();
	}

	return internalInvoke<ReturnType, ArgTypes...>(adjustCallerAddress(caller), std::forward<ArgTypes>(args)...);
}

template <typename ReturnType, typename CallerType, typename... ArgTypes, typename>
ReturnType Method::checkedInvoke(CallerType& caller, ArgTypes&&... args) const
{
	checkReturnType<ReturnType>();
	checkParameterTypes<ArgTypes...>();

	return internalInvoke<ReturnType, ArgTypes...>(checkedAdjustCallerAddress(caller), std::forward<ArgTypes>(args)...);
}

template <typename ReturnType, typename CallerType, typename... ArgTypes>
ReturnType Method::checkedInvoke(CallerType const& caller, ArgTypes&&... args) const
{
	checkReturnType<ReturnType>();
	checkParameterTypes<ArgTypes...>();

	if (!isConst())
	{
		throwConstViolationException();
	}

	return internalInvoke<ReturnType, ArgTypes...>(checkedAdjustCallerAddress(caller), std::forward<ArgTypes>(args)...);
}