#include "pch.h"
#include "CoreSystem.h"

CCoreSystem::CCoreSystem()
{

}

size_t CObject::sObjectType = 0;

CClass::CClass(const std::string& InClassName, ObjectType InClassType, std::function<CObject* (CActor*)> InCreateFunc)
	: ClassName(InClassName)
	, ClassType(InClassType)
	, CreateFunc(InCreateFunc)
{

}

CClassManager::CClassManager()
{

}