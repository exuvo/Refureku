#include "Refureku/TypeInfo/Archetypes/StructAPI.h"

#include "Refureku/TypeInfo/Archetypes/StructImpl.h"
#include "Refureku/TypeInfo/Archetypes/Enum.h"
#include "Refureku/TypeInfo/Entity/EntityUtility.h"

using namespace rfk;

template class REFUREKU_TEMPLATE_API_DEF rfk::Allocator<StructAPI const*>;
template class REFUREKU_TEMPLATE_API_DEF rfk::Vector<StructAPI const*, rfk::Allocator<StructAPI const*>>;

StructAPI::StructAPI(char const* name, std::size_t id, std::size_t memorySize, bool isClass, EClassKind classKind) noexcept:
	Archetype(new StructImpl(name, id, memorySize, isClass, classKind))
{
}

StructAPI::StructAPI(char const* name, std::size_t id, std::size_t memorySize, bool isClass) noexcept:
	StructAPI(name, id, memorySize, isClass, EClassKind::Standard)
{
}

StructAPI::StructAPI(StructImpl* implementation) noexcept:
	Archetype(implementation)
{
}

StructAPI::~StructAPI() noexcept = default;

rfk::Vector<StructAPI const*> StructAPI::getDirectSubclasses() const noexcept
{
	rfk::Vector<StructAPI const*> result;

	for (StructAPI const* subclass : reinterpret_cast<StructImpl const*>(getPimpl())->getSubclasses())
	{
		//Search this struct in subclasses's parents
		for (ParentStruct const& subclassParent : reinterpret_cast<StructImpl const*>(subclass->getPimpl())->getDirectParents())
		{
			if (&subclassParent.getArchetype() == this)
			{
				result.emplace_back(subclass);
				break;
			}
		}
	}

	return result;
}

bool StructAPI::isSubclassOf(StructAPI const& archetype) const noexcept
{
	return &archetype != this && archetype.isBaseOf(*this);
}

bool StructAPI::isBaseOf(StructAPI const& archetype) const noexcept
{
	auto const& subclasses = reinterpret_cast<StructImpl const*>(getPimpl())->getSubclasses();

	return &archetype == this || subclasses.find(&archetype) != subclasses.cend();
}

EClassKind StructAPI::getClassKind() const noexcept
{
	return reinterpret_cast<StructImpl const*>(getPimpl())->getClassKind();
}

ParentStruct const& StructAPI::getDirectParentAt(std::size_t index) const noexcept
{
	return reinterpret_cast<StructImpl const*>(getPimpl())->getDirectParents()[index];
}

std::size_t StructAPI::getDirectParentsCount() const noexcept
{
	return reinterpret_cast<StructImpl const*>(getPimpl())->getDirectParents().size();
}

StructAPI const* StructAPI::getNestedStructByName(char const* name, EAccessSpecifier access) const noexcept
{
	Archetype const* foundArchetype = reinterpret_cast<StructImpl const*>(getPimpl())->getNestedArchetype(name, access);

	return (foundArchetype != nullptr && foundArchetype->getKind() == EEntityKind::Struct) ?
				reinterpret_cast<StructAPI const*>(foundArchetype) :
				nullptr;
}

ClassAPI const* StructAPI::getNestedClassByName(char const* name, EAccessSpecifier access) const noexcept
{
	Archetype const* foundArchetype = reinterpret_cast<StructImpl const*>(getPimpl())->getNestedArchetype(name, access);

	return (foundArchetype != nullptr && foundArchetype->getKind() == EEntityKind::Class) ?
				reinterpret_cast<ClassAPI const*>(foundArchetype) :
				nullptr;
}

Enum const* StructAPI::getNestedEnumByName(char const* name, EAccessSpecifier access) const noexcept
{
	Archetype const* foundArchetype = reinterpret_cast<StructImpl const*>(getPimpl())->getNestedArchetype(name, access);

	return (foundArchetype != nullptr && foundArchetype->getKind() == EEntityKind::Enum) ?
				reinterpret_cast<Enum const*>(foundArchetype) :
				nullptr;
}

bool StructAPI::foreachNestedArchetype(Visitor<Archetype> visitor, void* userData) const
{
	return EntityUtility::foreachEntity(reinterpret_cast<StructImpl const*>(getPimpl())->getNestedArchetypes(), visitor, userData);
}

Field const* StructAPI::getFieldByName(char const* name, EFieldFlags minFlags, bool shouldInspectInherited) const noexcept
{
	Field const* result = nullptr;

	EntityUtility::foreachEntityNamed(reinterpret_cast<StructImpl const*>(getPimpl())->getFields(),
									  name,
									  [this, &result, minFlags, shouldInspectInherited](Field const& field)
									  {
										  /**
										  *	fields variable contains both this struct fields and inherited fields,
										  *	make sure we check inherited fields only if requested
										  */
										  if (shouldInspectInherited || field.getOuterEntity() == this)
										  {
											  if ((field.getFlags() & minFlags) == minFlags)
											  {
												  //We found a field that satisfies minFlags
												  result = &field;
												  return false;
											  }
										  }

										  return true;
									  });

	return result;
}

Vector<Field const*> StructAPI::getFieldsByName(char const* name, EFieldFlags minFlags, bool shouldInspectInherited) const noexcept
{
	//Users using this method likely are waiting for at least 2 results, so default capacity to 2.
	Vector<Field const*> result(2);

	EntityUtility::foreachEntityNamed(reinterpret_cast<StructImpl const*>(getPimpl())->getFields(),
									  name,
									  [this, &result, minFlags, shouldInspectInherited](Field const& field)
									  {
										  /**
										  *	fields variable contains both this struct fields and inherited fields,
										  *	make sure we check inherited fields only if requested
										  */
										  if (shouldInspectInherited || field.getOuterEntity() == this)
										  {
											  if ((field.getFlags() & minFlags) == minFlags)
											  {
												  //We found a field that satisfies minFlags
												  result.push_back(&field);
											  }
										  }

										  return true;
									  });

	return result;
}

bool StructAPI::foreachField(Visitor<Field> visitor, void* userData) const
{
	return EntityUtility::foreachEntity(reinterpret_cast<StructImpl const*>(getPimpl())->getFields(), visitor, userData);
}

StaticField const* StructAPI::getStaticFieldByName(char const* name, EFieldFlags minFlags, bool shouldInspectInherited) const noexcept
{
	StaticField const* result = nullptr;

	EntityUtility::foreachEntityNamed(reinterpret_cast<StructImpl const*>(getPimpl())->getStaticFields(),
									  name,
									  [this, &result, minFlags, shouldInspectInherited](StaticField const& staticField)
									  {
										  /**
										  *	static fields container contains both this struct static fields and inherited static fields,
										  *	make sure we check inherited fields only if requested
										  */
										  if (shouldInspectInherited || staticField.getOuterEntity() == this)
										  {
											  if ((staticField.getFlags() & minFlags) == minFlags)
											  {
												  //We found a static field that satisfies minFlags
												  result = &staticField;
												  return false;
											  }
										  }
										  
										  return true;
									  });

	return result;
}

Vector<StaticField const*> StructAPI::getStaticFieldsByName(char const* name, EFieldFlags minFlags, bool shouldInspectInherited) const noexcept
{
	//Users using this method likely are waiting for at least 2 results, so default capacity to 2.
	Vector<StaticField const*> result(2);

	EntityUtility::foreachEntityNamed(reinterpret_cast<StructImpl const*>(getPimpl())->getStaticFields(),
									  name,
									  [this, &result, minFlags, shouldInspectInherited](StaticField const& staticField)
									  {
										  /**
										  *	static fields container contains both this struct static fields and inherited static fields,
										  *	make sure we check inherited fields only if requested
										  */
										  if (shouldInspectInherited || staticField.getOuterEntity() == this)
										  {
											  if ((staticField.getFlags() & minFlags) == minFlags)
											  {
												  
												  result.push_back(&staticField);
											  }
										  }

										  return true;
									  });

	return result;
}

bool StructAPI::foreachStaticField(Visitor<StaticField> visitor, void* userData) const
{
	return EntityUtility::foreachEntity(reinterpret_cast<StructImpl const*>(getPimpl())->getStaticFields(), visitor, userData);
}

Method const* StructAPI::getMethodByName(char const* name, EMethodFlags minFlags, bool shouldInspectInherited) const noexcept
{
	StructImpl const*	structImpl	= reinterpret_cast<StructImpl const*>(getPimpl());
	Method const*	result		= nullptr;

	bool foundMethod = EntityUtility::foreachEntityNamed(structImpl->getMethods(),
									  name,
									  [&result, minFlags](Method const& method)
									  {
										  if ((method.getFlags() & minFlags) == minFlags)
										  {
											  //We found a method that satisfies minFlags
											  result = &method;
											  return false;
										  }

										  return true;
									  });

	if (foundMethod)
	{
		return result;
	}
	else
	{
		//If we reach this point, couldn't find a valid method
		if (shouldInspectInherited)
		{
			for (ParentStruct const& parent : structImpl->getDirectParents())
			{
				result = parent.getArchetype().getMethodByName(name, minFlags, true);

				if (result != nullptr)
				{
					return result;
				}
			}
		}

		return nullptr;
	}
}

Vector<Method const*> StructAPI::getMethodsByName(char const* name, EMethodFlags minFlags, bool shouldInspectInherited) const noexcept
{
	//Users using this method likely are waiting for at least 2 results, so default capacity to 2.
	Vector<Method const*>	result(2);
	StructImpl const*			structImpl	= reinterpret_cast<StructImpl const*>(getPimpl());

	EntityUtility::foreachEntityNamed(structImpl->getMethods(),
									 name,
									 [&result, minFlags](Method const& method)
									 {
										 if ((method.getFlags() & minFlags) == minFlags)
										 {
											 //We found a method that satisfies minFlags
											 result.push_back(&method);
										 }

										 return true;
									 });

	if (shouldInspectInherited)
	{
		for (ParentStruct const& parent : structImpl->getDirectParents())
		{
			result.push_back(parent.getArchetype().getMethodsByName(name, minFlags, true));
		}
	}

	return result;
}

bool StructAPI::foreachMethod(Visitor<Method> visitor, void* userData) const
{
	return EntityUtility::foreachEntity(reinterpret_cast<StructImpl const*>(getPimpl())->getMethods(), visitor, userData);
}

StaticMethod const* StructAPI::getStaticMethodByName(char const* name, EMethodFlags minFlags, bool shouldInspectInherited) const noexcept
{
	StructImpl const*		structImpl	= reinterpret_cast<StructImpl const*>(getPimpl());
	StaticMethod const*	result		= nullptr;

	bool foundMethod = EntityUtility::foreachEntityNamed(structImpl->getStaticMethods(),
														 name,
														 [&result, minFlags](StaticMethod const& staticMethod)
														 {
															 if ((staticMethod.getFlags() & minFlags) == minFlags)
															 {
																 //We found a static method that satisfies minFlags
																 result = &staticMethod;
																 return false;
															 }

															 return true;
														 });

	if (foundMethod)
	{
		return result;
	}
	else
	{
		//If we reach this point, couldn't find a valid static method
		if (shouldInspectInherited)
		{
			for (ParentStruct const& parent : structImpl->getDirectParents())
			{
				result = parent.getArchetype().getStaticMethodByName(name, minFlags, true);

				if (result != nullptr)
				{
					return result;
				}
			}
		}

		return nullptr;
	}
}

Vector<StaticMethod const*> StructAPI::getStaticMethodsByName(char const* name, EMethodFlags minFlags, bool shouldInspectInherited) const noexcept
{
	//Users using this method likely are waiting for at least 2 results, so default capacity to 2.
	Vector<StaticMethod const*>	result(2);
	StructImpl const*				structImpl = reinterpret_cast<StructImpl const*>(getPimpl());

	EntityUtility::foreachEntityNamed(structImpl->getStaticMethods(),
								   	 name,
								   	 [&result, minFlags](StaticMethod const& staticMethod)
								   	 {
								   		 if ((staticMethod.getFlags() & minFlags) == minFlags)
								   		 {
								   			 //We found a static method that satisfies minFlags
								   			 result.push_back(&staticMethod);
								   		 }
								   
								   		 return true;
								   	 });

	//If we reach this point, couldn't find a valid method
	if (shouldInspectInherited)
	{
		for (ParentStruct const& parent : structImpl->getDirectParents())
		{
			result.push_back(parent.getArchetype().getStaticMethodsByName(name, minFlags, true));
		}
	}

	return result;
}

bool StructAPI::foreachStaticMethod(Visitor<StaticMethod> visitor, void* userData) const
{
	return EntityUtility::foreachEntity(reinterpret_cast<StructImpl const*>(getPimpl())->getStaticMethods(), visitor, userData);
}

ClassTemplateAPI const* StructAPI::asTemplate() const noexcept
{
	return (getClassKind() == EClassKind::Template) ?
		reinterpret_cast<ClassTemplateAPI const*>(this) : nullptr;
}

ClassTemplateInstantiationAPI const* StructAPI::asTemplateInstantiation() const noexcept
{
	return (getClassKind() == EClassKind::TemplateInstantiation) ?
		reinterpret_cast<ClassTemplateInstantiationAPI const*>(this) : nullptr;
}

void StructAPI::addDirectParent(Archetype const* archetype, EAccessSpecifier inheritanceAccess) noexcept
{
	if (archetype != nullptr)
	{
		assert(archetype->getKind() == EEntityKind::Struct || archetype->getKind() == EEntityKind::Class);

		reinterpret_cast<StructImpl*>(getPimpl())->addDirectParent(*reinterpret_cast<StructAPI const*>(archetype), inheritanceAccess);
	}
}

void StructAPI::setDirectParentsCapacity(std::size_t capacity) noexcept
{
	reinterpret_cast<StructImpl*>(getPimpl())->setDirectParentsCapacity(capacity);
}

void StructAPI::addSubclass(StructAPI const& subclass) noexcept
{
	reinterpret_cast<StructImpl*>(getPimpl())->addSubclass(subclass);
}

Archetype* StructAPI::addNestedArchetype(Archetype const* nestedArchetype, EAccessSpecifier accessSpecifier) noexcept
{
	return reinterpret_cast<StructImpl*>(getPimpl())->addNestedArchetype(nestedArchetype, accessSpecifier, this);
}

void StructAPI::setNestedArchetypesCapacity(std::size_t capacity) noexcept
{
	reinterpret_cast<StructImpl*>(getPimpl())->setNestedArchetypesCapacity(capacity);
}

Field* StructAPI::addField(char const* name, std::size_t id, Type const& type,
							  EFieldFlags flags, std::size_t memoryOffset, StructAPI const* outerEntity) noexcept
{
	return reinterpret_cast<StructImpl*>(getPimpl())->addField(name, id, type, flags, this, memoryOffset, outerEntity);
}

void StructAPI::setFieldsCapacity(std::size_t capacity) noexcept
{
	return reinterpret_cast<StructImpl*>(getPimpl())->setFieldsCapacity(capacity);
}

StaticField* StructAPI::addStaticField(char const* name, std::size_t id, Type const& type,
										  EFieldFlags flags, void* fieldPtr, StructAPI const* outerEntity) noexcept
{
	return reinterpret_cast<StructImpl*>(getPimpl())->addStaticField(name, id, type, flags, this, fieldPtr, outerEntity);
}

StaticField* StructAPI::addStaticField(char const* name, std::size_t id, Type const& type,
										  EFieldFlags flags, void const* fieldPtr, StructAPI const* outerEntity) noexcept
{
	return reinterpret_cast<StructImpl*>(getPimpl())->addStaticField(name, id, type, flags, this, fieldPtr, outerEntity);
}

void StructAPI::setStaticFieldsCapacity(std::size_t capacity) noexcept
{
	return reinterpret_cast<StructImpl*>(getPimpl())->setStaticFieldsCapacity(capacity);
}

Method* StructAPI::addMethod(char const* name, std::size_t id,
								Type const& returnType, ICallable* internalMethod, EMethodFlags flags) noexcept
{
	return reinterpret_cast<StructImpl*>(getPimpl())->addMethod(name, id, returnType, internalMethod, flags, this);
}

void StructAPI::setMethodsCapacity(std::size_t capacity) noexcept
{
	return reinterpret_cast<StructImpl*>(getPimpl())->setMethodsCapacity(capacity);
}

StaticMethod* StructAPI::addStaticMethod(char const* name, std::size_t id,
											Type const& returnType, ICallable* internalMethod, EMethodFlags flags) noexcept
{
	return reinterpret_cast<StructImpl*>(getPimpl())->addStaticMethod(name, id, returnType, internalMethod, flags, this);
}

void StructAPI::setStaticMethodsCapacity(std::size_t capacity) noexcept
{
	return reinterpret_cast<StructImpl*>(getPimpl())->setStaticMethodsCapacity(capacity);
}

void StructAPI::setDefaultInstantiator(void* (*instantiator)()) noexcept
{
	reinterpret_cast<StructImpl*>(getPimpl())->setDefaultInstantiator(instantiator);
}

void StructAPI::addInstantiator(StaticMethod const* instantiator) noexcept
{
	reinterpret_cast<StructImpl*>(getPimpl())->addInstantiator(instantiator);
}

void* StructAPI::makeInstanceFromDefaultInstantiator() const
{
	void* (*defaultInstantiator)() = reinterpret_cast<StructImpl const*>(getPimpl())->getDefaultInstantiator();

	assert(defaultInstantiator != nullptr);

	return (*defaultInstantiator)();
}

std::size_t StructAPI::getInstantiatorsCount() const noexcept
{
	return reinterpret_cast<StructImpl const*>(getPimpl())->getCustomInstantiators().size();
}

StaticMethod const* StructAPI::getInstantiatorAt(std::size_t index) const noexcept
{
	return reinterpret_cast<StructImpl const*>(getPimpl())->getCustomInstantiators()[index];
}