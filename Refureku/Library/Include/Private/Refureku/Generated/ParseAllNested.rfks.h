#pragma once

#include "../../../Public/Refureku/Properties/ParseAllNested.h"

#include <type_traits>
#include <Refureku/TypeInfo/Variables/Variable.h>
#include <Refureku/TypeInfo/Functions/Function.h>
#include <Refureku/TypeInfo/Entity/DefaultEntityRegisterer.h>
#include <Refureku/TypeInfo/Archetypes/ArchetypeRegisterer.h>
#include <Refureku/TypeInfo/Namespace/Namespace.h>
#include <Refureku/TypeInfo/Namespace/NamespaceFragment.h>
#include <Refureku/TypeInfo/Namespace/NamespaceFragmentRegisterer.h>
#include <Refureku/TypeInfo/Archetypes/Template/TemplateParameterAPI.h>

rfk::EEntityKind ParseAllNested::getTargetEntityKind() const noexcept { return targetEntityKind; }
static_assert(std::is_base_of_v<rfk::Property, ParseAllNested>, "[Refureku] Can't attach PropertySettings property to ParseAllNested as it doesn't inherit from rfk::Property.");
namespace rfk::generated { static rfk::ArchetypeRegisterer registerer_17636175816175907530u = ParseAllNested::staticGetArchetype(); }

rfk::Class const& ParseAllNested::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Class type("ParseAllNested", 17636175816175907530u, sizeof(ParseAllNested), 1);
if (!initialized) {
initialized = true;
type.setDefaultInstantiator(&rfk::internal::defaultInstantiator<ParseAllNested>);
type.setPropertiesCapacity(1);
static_assert((PropertySettings::targetEntityKind & rfk::EEntityKind::Class) != rfk::EEntityKind::Undefined, "[Refureku] PropertySettings can't be applied to a rfk::EEntityKind::Class");static PropertySettings property_17636175816175907530u_PropertySettings_0{rfk::EEntityKind::Namespace | rfk::EEntityKind::Class | rfk::EEntityKind::Struct};type.addProperty(&property_17636175816175907530u_PropertySettings_0);
type.setDirectParentsCapacity(1);
type.addDirectParent(rfk::getArchetype<rfk::Property>(), static_cast<rfk::EAccessSpecifier>(1));
ParseAllNested::_rfk_registerChildClass<ParseAllNested>(type);
}
return type; }

rfk::Class const& ParseAllNested::getArchetype() const noexcept { return ParseAllNested::staticGetArchetype(); }

template <> rfk::Archetype const* rfk::getArchetype<ParseAllNested>() noexcept { return &ParseAllNested::staticGetArchetype(); }


