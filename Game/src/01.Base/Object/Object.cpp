#include "pch.h"
#include "Object.h"
#include "Actor.h"

size_t CObject::sObjectType = 0;

CClass::CClass(const std::string& InClassName, std::function<CObject* (CActor*)> InCreateFunc)
	: ClassName(InClassName)
	, CreateFunc(InCreateFunc)
{
	
}

CObject* CClass::CreateObject(CActor* InOwnerActor)
{
	return CreateFunc(InOwnerActor);
}

CClassManager::CClassManager()
{

}