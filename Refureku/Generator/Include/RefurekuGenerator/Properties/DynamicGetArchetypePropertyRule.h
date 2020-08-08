/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <Kodgen/Properties/DefaultSimplePropertyRule.h>

namespace rfk
{
	class DynamicGetArchetypePropertyRule : public kodgen::DefaultSimplePropertyRule
	{
		public:
			DynamicGetArchetypePropertyRule()										noexcept;
			DynamicGetArchetypePropertyRule(DynamicGetArchetypePropertyRule const&) = default;
			DynamicGetArchetypePropertyRule(DynamicGetArchetypePropertyRule&&)		= default;
			virtual ~DynamicGetArchetypePropertyRule()								= default;

			virtual bool	isPropertyGroupValid(kodgen::PropertyGroup const&	propertyGroup,
												 kodgen::uint8					propertyIndex,
												 std::string&					out_errorDescription)	const noexcept override;
	};
}