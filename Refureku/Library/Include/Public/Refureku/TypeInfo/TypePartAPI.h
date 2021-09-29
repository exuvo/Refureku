/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Refureku/Config.h"
#include "Refureku/TypeInfo/ETypePartDescriptor.h"

namespace rfk
{
	class TypePartAPI
	{
		public:
			using AdditionalDataType = uint32;

		private:
			/** Some additional data which might complement descriptor (for example CArray size) */
			AdditionalDataType	_additionalData	= 0u;

			/** Actual data describing this type part */
			ETypePartDescriptor	_descriptor		= ETypePartDescriptor::Undefined;

			/**
			*	Padding to make sure that the class takes 8 complete bytes of fully initialized memory.
			*	Used later to compare lists of TypePartAPI using std::memcmp.
			*/
			uint16				_padding		= 0u;

		public:
			TypePartAPI()									= default;
			TypePartAPI(ETypePartDescriptor	descriptor,
						AdditionalDataType	additionalData)	noexcept;

			/**
			*	@brief	Add a flag to the descriptor field.
			* 
			*	@param flag The flag to add this this type part descriptor.
			*/
			REFUREKU_API void				addDescriptorFlag(ETypePartDescriptor flag)	noexcept;

			REFUREKU_API bool				isPointer()							const	noexcept;
			REFUREKU_API bool				isLValueReference()					const	noexcept;
			REFUREKU_API bool				isRValueReference()					const	noexcept;
			REFUREKU_API bool				isCArray()							const	noexcept;
			REFUREKU_API bool				isValue()							const	noexcept;
			REFUREKU_API bool				isConst()							const	noexcept;
			REFUREKU_API bool				isVolatile()						const	noexcept;
			REFUREKU_API AdditionalDataType	getCArraySize()						const	noexcept;

			/**
			*	@brief Setter for the field _additionalData.
			* 
			*	@param data The data to set.
			*/
			REFUREKU_API void				setAdditionalData(AdditionalDataType data)	noexcept;
	};

	static_assert(sizeof(TypePartAPI) == 8u, "TypePartAPI must takes 8 bytes of fully initialized memory to allow the use of std::memcmp.");
}