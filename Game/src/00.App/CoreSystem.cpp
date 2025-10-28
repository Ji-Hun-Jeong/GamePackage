#include "pch.h"
#include "CoreSystem.h"

CCoreSystem::CCoreSystem()
{

}

void DestroyObject(CObject* InObject)
{
	CCoreSystem::GetInst().DeregistObject(InObject);
}