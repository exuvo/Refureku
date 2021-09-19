#pragma once

#include "../TypeInfo/Properties/ParseAllNested.h"

#include <type_traits>
#include <Refureku/TypeInfo/Namespaces/Namespace.h>
#include <Refureku/TypeInfo/Namespaces/NamespaceFragment.h>
#include <Refureku/TypeInfo/Archetypes/TemplateParameter.h>
rfk::EEntityKind ParseAllNested::getTargetEntityKind() const noexcept { return targetEntityKind; }
static_assert(std::is_base_of_v<rfk::Property, ParseAllNested>, "[Refureku] Can't attach PropertySettings property to ParseAllNested as it doesn't inherit from rfk::Property.");
rfk::ArchetypeRegisterer rfk::generated::_rfk_archetypeRegisterer_17636175816175907530u = ParseAllNested::staticGetArchetype(); 

rfk::Struct const& ParseAllNested::staticGetArchetype() noexcept {
static bool initialized = false;
static rfk::Struct type("ParseAllNested", 17636175816175907530u, sizeof(ParseAllNested), false);
if (!initialized) {
initialized = true;
type.setDefaultInstantiator(&rfk::defaultInstantiator<ParseAllNested>);
type.properties.reserve(1);
static_assert((PropertySettings::targetEntityKind & rfk::EEntityKind::Struct) != rfk::EEntityKind::Undefined, "[Refureku] PropertySettings can't be applied to a rfk::EEntityKind::Struct");static PropertySettings property_17636175816175907530u_PropertySettings_0{rfk::EEntityKind::Namespace | rfk::EEntityKind::Class | rfk::EEntityKind::Struct};type.properties.emplace_back(&property_17636175816175907530u_PropertySettings_0);
type.directParents.reserve(1);
type.addToParents<rfk::Property>(static_cast<rfk::EAccessSpecifier>(1));
ParseAllNested::_rfk_registerChildClass<ParseAllNested>(type);
}
return type; }

rfk::Struct const& ParseAllNested::getArchetype() const noexcept { return ParseAllNested::staticGetArchetype(); }

template <> rfk::Archetype const* rfk::getArchetype<ParseAllNested>() noexcept { return &ParseAllNested::staticGetArchetype(); }


