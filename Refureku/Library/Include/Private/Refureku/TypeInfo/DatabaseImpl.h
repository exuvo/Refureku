/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <unordered_set>
#include <vector>
#include <memory>	//std::shared_ptr
#include <cassert>

#include "Refureku/TypeInfo/DatabaseAPI.h"
#include "Refureku/TypeInfo/Entity/EntityHash.h"
#include "Refureku/TypeInfo/Namespace/NamespaceAPI.h"
#include "Refureku/TypeInfo/Archetypes/StructAPI.h"
#include "Refureku/TypeInfo/Archetypes/EnumAPI.h"
#include "Refureku/TypeInfo/Archetypes/EnumValueAPI.h"
#include "Refureku/TypeInfo/Variables/VariableAPI.h"
#include "Refureku/TypeInfo/Variables/FieldAPI.h"
#include "Refureku/TypeInfo/Variables/StaticFieldAPI.h"
#include "Refureku/TypeInfo/Functions/FunctionAPI.h"
#include "Refureku/TypeInfo/Functions/MethodAPI.h"
#include "Refureku/TypeInfo/Functions/StaticMethodAPI.h"
#include "Refureku/TypeInfo/Archetypes/FundamentalArchetypeAPI.h"

namespace rfk
{
	class DatabaseAPI::DatabaseImpl final
	{
		public:
			using EntitiesById					= std::unordered_set<EntityAPI const*, EntityPtrIdHash, EntityPtrIdEqual>;
			using NamespacesByName				= std::unordered_set<NamespaceAPI const*, EntityPtrNameHash, EntityPtrNameEqual>;
			using StructsByName					= std::unordered_set<StructAPI const*, EntityPtrNameHash, EntityPtrNameEqual>;
			using ClassesByName					= std::unordered_set<ClassAPI const*, EntityPtrNameHash, EntityPtrNameEqual>;
			using EnumsByName					= std::unordered_set<EnumAPI const*, EntityPtrNameHash, EntityPtrNameEqual>;
			using VariablesByName				= std::unordered_set<VariableAPI const*, EntityPtrNameHash, EntityPtrNameEqual>;
			using FunctionsByName				= std::unordered_multiset<FunctionAPI const*, EntityPtrNameHash, EntityPtrNameEqual>;
			using FundamentalArchetypesByName	= std::unordered_set<FundamentalArchetypeAPI const*, EntityPtrNameHash, EntityPtrNameEqual>;
			using GenNamespaces					= std::vector<std::shared_ptr<NamespaceAPI>>;
			
		private:
			/** Collection of all registered entities hashed by Id.  */
			EntitiesById				_entitiesById;

			/** Collection of all file level namespaces hashed by name. */
			NamespacesByName			_fileLevelNamespacesByName;

			/** Collection of all file level structs hashed by name. */
			StructsByName				_fileLevelStructsByName;

			/** Collection of all file level classes hashed by name. */
			ClassesByName				_fileLevelClassesByName;

			/** Collection of all file level enums hashed by name. */
			EnumsByName					_fileLevelEnumsByName;

			/** Collection of all file level variables hashed by name. */
			VariablesByName				_fileLevelVariablesByName;

			/** Collection of all file level functions hashed by name. */
			FunctionsByName				_fileLevelFunctionsByName;

			/** Collection of all fundamental archetypes hashed by name. */
			FundamentalArchetypesByName	_fundamentalArchetypes;

			/** Collection of namespace objects generated by the database. */
			GenNamespaces				_generatedNamespaces;

			/**
			*	@brief Add all nested entities to the _entitiesById map.
			*	
			*	@param s The parent struct.
			*/
			inline void		registerSubEntities(StructAPI const& s)		noexcept;

			/**
			*	@brief Remove all nested entities from the _entitiesById map.
			*	
			*	@param s The parent struct.
			*/
			inline void		unregisterSubEntities(StructAPI const& s)	noexcept;

			/**
			*	@brief Add all nested entities to the _entitiesById map.
			*	
			*	@param e The parent enum.
			*/
			inline void		registerSubEntities(EnumAPI const& e)		noexcept;

			/**
			*	@brief Remove all nested entities from the _entitiesById map.
			*	
			*	@param e The parent enum.
			*/
			inline void		unregisterSubEntities(EnumAPI const& e)		noexcept;

		public:
			DatabaseImpl()	= default;
			~DatabaseImpl()	= default;
			
			/**
			*	@brief Register a file level entity to the database (add it to both _entitiesById & _fileLevelEntitiesByName).
			*	
			*	@param entity						The entity to register.
			*	@param shouldRegisterSubEntities	Should sub entities be registered by id recursively?
			*/
			inline void											registerFileLevelEntity(EntityAPI const&	entity,
																						bool				shouldRegisterSubEntities)	noexcept;

			/**
			*	@brief Register an entity to the database.
			*	
			*	@param entity						The entity to register.
			*	@param shouldRegisterSubEntities	Should sub entities be registered recursively?
			*/
			inline void											registerEntityId(EntityAPI const&	entity,
																				 bool				shouldRegisterSubEntities)			noexcept;

			/**
			*	@brief Unregister an entity from the database.
			*	
			*	@param entity						The entity to unregister.
			*	@param shouldUnregisterSubEntities	Should sub entities be unregistered recursively?
			*/
			inline void											unregisterEntity(EntityAPI const&	entity,
																				 bool				shouldUnregisterSubEntities)		noexcept;

			/**
			*	@brief	Check that a namespace is still referenced by some namespace fragment.
			*			If not, remove it from the database.
			*
			*	@param npPtr Pointer to the namespace to check.
			*/
			inline void											checkNamespaceRefCount(std::shared_ptr<NamespaceAPI> const& npPtr)		noexcept;

			/**
			*	@brief Generate a namespace object to share between different namespace fragments.
			*	
			*	@param name Name of the namespace to generate.
			*	@param id Id of the namespace to generate.
			*
			*	@return The newly generated namespace.
			*/
			RFK_NODISCARD inline std::shared_ptr<NamespaceAPI>	generateNamespace(char const*	name,
																				  std::size_t	id)										noexcept;

			/**
			*	@brief Getters for each field.
			*/
			RFK_NODISCARD inline EntitiesById const&				getEntitiesById()					const	noexcept;
			RFK_NODISCARD inline NamespacesByName const&			getFileLevelNamespacesByName()		const	noexcept;
			RFK_NODISCARD inline StructsByName const&				getFileLevelStructsByName()			const	noexcept;
			RFK_NODISCARD inline ClassesByName const&				getFileLevelClassesByName()			const	noexcept;
			RFK_NODISCARD inline EnumsByName const&					getFileLevelEnumsByName()			const	noexcept;
			RFK_NODISCARD inline VariablesByName const&				getFileLevelVariablesByName()		const	noexcept;
			RFK_NODISCARD inline FunctionsByName const&				getFileLevelFunctionsByName()		const	noexcept;
			RFK_NODISCARD inline FundamentalArchetypesByName const&	getFundamentalArchetypesByName()	const	noexcept;
			RFK_NODISCARD inline GenNamespaces const&				getGeneratedNamespaces()			const	noexcept;
	};

	#include "Refureku/TypeInfo/DatabaseImpl.inl"
}