#include "Refureku/TypeInfo/Archetypes/Enum.h"

#include <cassert>

#include "Refureku/TypeInfo/Type.h"

using namespace rfk;

Enum::Enum(std::string&& name, std::size_t id, uint64 memorySize, Archetype const* underlyingType, Entity const* outerEntity) noexcept:
	Archetype(std::forward<std::string>(name), id, EEntityKind::Enum, memorySize, outerEntity),
	_underlyingType{*underlyingType}
{
	assert(underlyingType != nullptr);
}

EnumValue const* Enum::getEnumValue(std::string enumValueName)  const noexcept
{
	decltype(_values)::const_iterator it = _values.find(static_cast<EnumValue&&>(Entity(std::move(enumValueName), 0u)));

	return (it != _values.cend()) ? &*it : nullptr;
}

EnumValue const* Enum::getEnumValue(int64 value) const noexcept
{
	decltype(_values)::const_iterator it = std::find_if(_values.cbegin(), _values.cend(), [value](EnumValue const& v)
													   { return v.value == value; });

	return (it != _values.cend()) ? &*it : nullptr;
}

std::vector<EnumValue const*> Enum::getEnumValues(int64 value) const noexcept
{
	std::vector<EnumValue const*> result;

	for (EnumValue const& ev : _values)
	{
		if (ev.value == value)
		{
			result.push_back(&ev);
		}
	}

	return result;
}

EnumValue* Enum::addEnumValue(std::string enumValueName, uint64 entityId, int64 value) noexcept
{
	//Add the enum value to the container
	return const_cast<EnumValue*>(&*_values.emplace(std::move(enumValueName), entityId, value, this).first);
}

std::unordered_set<EnumValue, Entity::NameHasher, Entity::EqualName> const& Enum::getEnumValues() const noexcept
{
	return _values;
}

std::unordered_set<EnumValue, Entity::NameHasher, Entity::EqualName>& Enum::getEnumValues() noexcept
{
	return _values;
}

Archetype const& Enum::getUnderlyingType() const noexcept
{
	return _underlyingType;
}