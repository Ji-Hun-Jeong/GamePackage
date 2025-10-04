#pragma once
#include "../Object.h"

class CActor : public CObject
{
	DONTCOPY(CActor)
public:
	CActor() {}
	virtual ~CActor() {}



public:
	virtual void Update(float InDeltaTime)
	{
		// 자신 업뎃 하고 ActorComponent업데이트
	}

};

