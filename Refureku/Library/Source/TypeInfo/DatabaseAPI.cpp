#include "Refureku/TypeInfo/DatabaseAPI.h"

#include <string>

#include "Refureku/TypeInfo/DatabaseImpl.h"
#include "Refureku/TypeInfo/Entity/EntityUtility.h"
#include "Refureku/TypeInfo/Entity/EntityCastAPI.h"
#include "Refureku/Exceptions/BadNamespaceFormat.h"

using namespace rfk;

DatabaseAPI::DatabaseAPI() noexcept:
	_pimpl(new DatabaseImpl())
{
}

DatabaseAPI::~DatabaseAPI() noexcept = default;

DatabaseAPI& DatabaseAPI::getInstance() noexcept
{
	static DatabaseAPI database;
	
	return database;
}

EntityAPI const* DatabaseAPI::getEntityById(std::size_t id) const noexcept
{
	return EntityUtility::getEntityPtrById(_pimpl->getEntitiesById(), id);
}

NamespaceAPI const* DatabaseAPI::getNamespaceById(std::size_t id) const noexcept
{
	return entityCast<NamespaceAPI>(getEntityById(id));
}

NamespaceAPI const* DatabaseAPI::getNamespaceByName(char const* name) const
{
	std::string namespaceName(name);

	std::size_t index = namespaceName.find_first_of(':');

	//Make sure namespaceName has a valid namespace syntax
	if (index != std::string::npos && (index == namespaceName.size() - 1 || namespaceName[index + 1] != ':'))
	{
		throw BadNamespaceFormat("The provided namespace name is ill formed.");
	}

	NamespaceAPI const* result = EntityUtility::getEntityPtrByNameAndPredicate(_pimpl->getFileLevelNamespacesByName(), namespaceName.substr(0u, index).data(),
																			   [](NamespaceAPI const*) { return true; });

	//Couldn't find first namespace part, abort search
	if (result == nullptr)
	{
		return nullptr;
	}

	while (index != std::string::npos && result != nullptr)
	{
		if (namespaceName.size() <= index + 2u ||	//The provided namespace name either ends with : or :[some char]
			namespaceName[index + 1] != ':')		//or the namespace separation was : instead of ::
		{
			throw BadNamespaceFormat("The provided namespace name is ill formed.");
		}

		//Remove namespace separation :: 
		namespaceName	= namespaceName.substr(index + 2u);
		index			= namespaceName.find_first_of(':');

		result = result->getNamespaceByName(namespaceName.substr(0u, index).data());
	}

	return result;
}

ArchetypeAPI const* DatabaseAPI::getArchetypeById(std::size_t id) const noexcept
{
	return entityCast<ArchetypeAPI>(getEntityById(id));
}

ArchetypeAPI const* DatabaseAPI::getArchetypeByName(char const* name) const noexcept
{
	ArchetypeAPI const* result = getClassByName(name);

	if (result == nullptr)
	{
		result = getStructByName(name);

		if (result == nullptr)
		{
			result = getEnumByName(name);

			if (result == nullptr)
			{
				result = getFundamentalArchetypeByName(name);
			}
		}
	}

	return result;
}

StructAPI const* DatabaseAPI::getStructById(std::size_t id) const noexcept
{
	return entityCast<StructAPI>(getEntityById(id));
}

StructAPI const* DatabaseAPI::getStructByName(char const* name) const noexcept
{
	return EntityUtility::getEntityPtrByNameAndPredicate(_pimpl->getFileLevelStructsByName(), name, [](StructAPI const*) { return true; });
}

ClassAPI const* DatabaseAPI::getClassById(std::size_t id) const noexcept
{
	return entityCast<ClassAPI>(getEntityById(id));
}

ClassAPI const* DatabaseAPI::getClassByName(char const* name) const noexcept
{
	return EntityUtility::getEntityPtrByNameAndPredicate(_pimpl->getFileLevelClassesByName(), name, [](ClassAPI const*) { return true; });
}

EnumAPI const* DatabaseAPI::getEnumById(std::size_t id) const noexcept
{
	return entityCast<EnumAPI>(getEntityById(id));
}

EnumAPI const* DatabaseAPI::getEnumByName(char const* name) const noexcept
{
	return EntityUtility::getEntityPtrByNameAndPredicate(_pimpl->getFileLevelEnumsByName(), name, [](EnumAPI const*) { return true; });
}

FundamentalArchetypeAPI const* DatabaseAPI::getFundamentalArchetypeById(std::size_t id) const noexcept
{
	return entityCast<FundamentalArchetypeAPI>(getEntityById(id));
}

FundamentalArchetypeAPI const* DatabaseAPI::getFundamentalArchetypeByName(char const* name) const noexcept
{
	return EntityUtility::getEntityPtrByNameAndPredicate(_pimpl->getFundamentalArchetypesByName(), name, [](FundamentalArchetypeAPI const*) { return true; });
}

VariableAPI const* DatabaseAPI::getVariableById(std::size_t id) const noexcept
{
	return entityCast<VariableAPI>(getEntityById(id));
}

VariableAPI const* DatabaseAPI::getVariableByName(char const* name, EVarFlags flags) const noexcept
{
	return EntityUtility::getEntityPtrByNameAndPredicate(_pimpl->getFileLevelVariablesByName(), name,
														 [flags](VariableAPI const* var) { return (var->getFlags() & flags) == flags; });
}

FunctionAPI const* DatabaseAPI::getFunctionById(std::size_t id) const noexcept
{
	return entityCast<FunctionAPI>(getEntityById(id));
}

FunctionAPI const* DatabaseAPI::getFunctionByName(char const* name, EFunctionFlags flags) const noexcept
{
	return EntityUtility::getEntityPtrByNameAndPredicate(_pimpl->getFileLevelFunctionsByName(), name,
														 [flags](FunctionAPI const* func) { return (func->getFlags() & flags) == flags; });
}

MethodAPI const* DatabaseAPI::getMethodById(std::size_t id) const noexcept
{
	return entityCast<MethodAPI>(getEntityById(id));
}

StaticMethodAPI const* DatabaseAPI::getStaticMethodById(std::size_t id) const noexcept
{
	return entityCast<StaticMethodAPI>(getEntityById(id));
}

FieldAPI const* DatabaseAPI::getFieldById(std::size_t id) const noexcept
{
	return entityCast<FieldAPI>(getEntityById(id));
}

StaticFieldAPI const* DatabaseAPI::getStaticFieldById(std::size_t id) const noexcept
{
	return entityCast<StaticFieldAPI>(getEntityById(id));
}

EnumValueAPI const* DatabaseAPI::getEnumValueById(std::size_t id) const noexcept
{
	return entityCast<EnumValueAPI>(getEntityById(id));
}

DatabaseAPI const& rfk::getDatabaseAPI() noexcept
{
	return DatabaseAPI::getInstance();
}