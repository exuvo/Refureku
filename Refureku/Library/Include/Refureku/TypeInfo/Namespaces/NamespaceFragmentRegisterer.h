/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <string>
#include <memory>

#include "Refureku/Config.h"
#include "Refureku/Misc/FundamentalTypes.h"

namespace rfk
{
	//Forward declarations
	class Namespace;			
	class NamespaceFragment;

	class NamespaceFragmentRegisterer
	{
		private:
			/** Fragment registered by this registerer. */
			NamespaceFragment const&	_fragment;

			/** Namespace this fragment is bound to. */
			std::shared_ptr<Namespace>	_namespaceInstance	= nullptr;

			/**
			*	@brief Add namespace fragment entities to the namespace held by this registerer.
			*	
			*	@param fragment The namespace fragment to merge.
			*/
			void	mergeFragmentToNamespace(NamespaceFragment const& fragment)						noexcept;

			/**
			*	@brief Append fragment properties to the namespace properties.
			*	
			*	@param fragment	The namespace fragment to merge.
			*/
			void	mergeFragmentPropertiesToNamespaceProperties(NamespaceFragment const& fragment)	noexcept;

			/**
			*	@brief Remove namespace fragment entities from the namespace held by this registerer.
			*	
			*	@param fragment The namespace fragment to remove.
			*/
			void	removeFragmentFromNamespace(NamespaceFragment const& fragment)					noexcept;

		public:
			REFUREKU_API NamespaceFragmentRegisterer(char const*	name,
										std::size_t					id,
										NamespaceFragment const&	namespaceFragment,
										bool						isFileLevelNamespace)	noexcept;
			REFUREKU_API ~NamespaceFragmentRegisterer()										noexcept;

			NamespaceFragmentRegisterer(NamespaceFragmentRegisterer const&)					= delete;
			NamespaceFragmentRegisterer(NamespaceFragmentRegisterer&&)						= delete;

			/**
			*	@brief Getter for _namespaceInstance.
			*	
			*	@return _namespaceInstance
			*/
			REFUREKU_API Namespace const* getNamespaceInstance()	const	noexcept;

			NamespaceFragmentRegisterer& operator=(NamespaceFragmentRegisterer const&)	= delete;
			NamespaceFragmentRegisterer& operator=(NamespaceFragmentRegisterer&&)		= delete;
	};
}