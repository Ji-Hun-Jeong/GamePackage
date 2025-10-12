#include "pch.h"
#include "Object.h"

#include "../World/World.h"

#include "03.Utils/NumberGenerator/NumberGenerator.h"

void CObject::Destroy()
{
	World->MarkDestroyed();

	for (auto& Child : Childs)
		Child->Destroy();

	bDestroy = true;

	EndPlay();

	if (Owner)
		Owner->DetachChild(this);
}
