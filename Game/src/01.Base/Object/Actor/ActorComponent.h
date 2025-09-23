#pragma once
#include "../Object/Object.h"

class ActorComponent : public CObject
{
	virtual void Update(float InDeltaTime) = 0;
};

