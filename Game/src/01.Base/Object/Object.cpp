#include "pch.h"
#include "Object.h"
#include "Actor.h"

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