#include "pch.h"
#include "Object.h"

#include "../World/World.h"

#include "03.Utils/NumberGenerator/NumberGenerator.h"

size_t CObject::sObjectType = 0;
void CObject::Destroy()
{
	World->MarkDestroyed();

	bDestroy = true;

	EndPlay();
}
