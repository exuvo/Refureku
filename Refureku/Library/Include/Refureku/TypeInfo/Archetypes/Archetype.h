/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <type_traits>

#include "Refureku/Config.h"
#include "Refureku/TypeInfo/Entity/Entity.h"
#include "Refureku/TypeInfo/EAccessSpecifier.h"

namespace rfk
{
	class Archetype : public Entity
	{
		private:
			/** Access specifier of this archetype. Relevant only when this archetype is nested (Undefined otherwise). */
			EAccessSpecifier	_accessSpecifier	= EAccessSpecifier::Undefined;

			/** Size in bytes an instance of this archetype takes in memory, basically what sizeof(Type) returns */
			std::size_t			_memorySize			= 0;

		protected:
			Archetype(std::string&& newName,
					  std::size_t	id,
					  EEntityKind	kind,
					  std::size_t	memorySize,
					  Entity const*	outerEntity = nullptr)	noexcept;
			Archetype(Archetype const&)						= delete;
			Archetype(Archetype&&)							= delete;
			~Archetype()									= default;

		public:
			/**
			*	@brief Getter for the field _accessSpecifier.
			* 
			*	@return _accessSpecifier.
			*/
			REFUREKU_API EAccessSpecifier	getAccessSpecifier()			const	noexcept;

			/**
			*	@brief Setter for the field _accessSpecifier.
			* 
			*	@param The access specifier to set.
			*/
			REFUREKU_API void				setAccessSpecifier(EAccessSpecifier)	noexcept;

			/**
			*	@brief Getter for the field _memorySize.
			* 
			*	@return _memorySize.
			*/
			REFUREKU_API std::size_t		getMemorySize()					const	noexcept;
	};

	#include "Refureku/TypeInfo/Archetypes/Archetype.inl"
}