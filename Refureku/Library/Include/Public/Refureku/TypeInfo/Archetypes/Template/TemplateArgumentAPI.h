/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include "Refureku/Config.h"
#include "Refureku/Utility/Pimpl.h"

namespace rfk
{
	//Forward declaration
	class TemplateParameterAPI;
	class ArchetypeAPI;	//TODO: Replace this by a template arg

	class TemplateArgumentAPI final
	{
		public:
			REFUREKU_API TemplateArgumentAPI(TemplateParameterAPI const& boundParameter)	noexcept;
			TemplateArgumentAPI(TemplateArgumentAPI const&)									= delete;
			TemplateArgumentAPI(TemplateArgumentAPI&&)										= delete;
			REFUREKU_API ~TemplateArgumentAPI()												noexcept;

			/**
			*	@brief Getter for the field _boundParameter.
			* 
			*	@return _boundParameter.
			*/
			RFK_NODISCARD REFUREKU_API TemplateParameterAPI const&	getBoundParameter()	const	noexcept;

			/**
			*	@brief Getter for the field _archetype.
			* 
			*	@return _archetype.
			*/
			//TODO: This will change to a different return type to support non-type template and template template type arguments
			RFK_NODISCARD REFUREKU_API ArchetypeAPI const*			getArchetype()		const	noexcept;

		private:
			//Forward declaration
			class TemplateArgumentImpl;

			/** Pointer to the concrete TemplateParameter implementation. */
			Pimpl<TemplateArgumentImpl>	_pimpl;
	};
}