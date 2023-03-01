/**
*	Copyright (c) 2021 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Refureku library project which is released under the MIT License.
*	See the LICENSE.md file for full license details.
*/

#pragma once

#include "Refureku/Object.h"
#include "TestClass.h"
#include "NonReflectedClass.h"
#include "ConstructionTrackedClass.h"

#include "Generated/TestFields.rfkh.h"

class ForwardDeclaredClass;

class CLASS() TestFieldsClass : public rfk::Object
{
	public:
		FIELD()
		int								intField					= 42;

		FIELD()
		int const						constIntField				= 314;

		FIELD()
		TestClass						testClassField;

		FIELD()
		ForwardDeclaredClass*			forwardDeclaredClassField	= nullptr;

		FIELD()
		NonReflectedClass				nonReflectedClassField;

		FIELD()
		ConstructionTrackedClass		ctorTrackedClassField;

		FIELD()
		ConstructionTrackedClass const	constCtorTrackedClassField;

	TestFieldsClass_GENERATED
};

class CLASS() TestFieldsClass2 : public rfk::Object
{
	public:
		FIELD()
		int	intField2 = 2;

	TestFieldsClass2_GENERATED
};

class CLASS() TestFieldsClassChild : public TestFieldsClass, public TestFieldsClass2
{
	public:
		FIELD()
		int	intField3 = 3;

	TestFieldsClassChild_GENERATED
};

class CLASS() TestFieldsUnrelatedClass : public rfk::Object
{
	TestFieldsUnrelatedClass_GENERATED
};

File_TestFields_GENERATED