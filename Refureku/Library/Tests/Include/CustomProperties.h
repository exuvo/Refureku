#pragma once

#include <utility>	//std::move

#include <Refureku/TypeInfo/Properties/PropertySettings.h>

#include "Generated/CustomProperties.rfk.h"

struct RFKStruct(PropertySettings(rfk::EEntityKind::Field | rfk::EEntityKind::Class, false, true)) CustomProperty : public rfk::Property
{
	RFKField()
	int i = 0;

	RFKField()
	int j = 0;

	CustomProperty() = default;
	CustomProperty(int _i, int _j) noexcept: i{_i}, j{_j} {}

	CustomProperty_GENERATED
};

struct RFKStruct(PropertySettings(rfk::EEntityKind::Class, true, false)) CustomProperty2 : public CustomProperty
{
	CustomProperty2_GENERATED 
};

struct RFKStruct(PropertySettings(rfk::EEntityKind::Method)) Tooltip : public rfk::Property 
{
	std::string message;

	Tooltip(std::string _message) noexcept:
		message{std::move(_message)}
	{}

	Tooltip_GENERATED
};

struct RFKStruct(PropertySettings(rfk::EEntityKind::Field | rfk::EEntityKind::Variable, false)) Slider : public rfk::Property
{
	float min;
	float max;
	float step;

	Slider(float min, float max, float step) noexcept:
		min{min},
		max{max},
		step{step}
	{}

	Slider_GENERATED
};

File_GENERATED