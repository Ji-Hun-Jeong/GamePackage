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
		CObject::Update(InDeltaTime);

	}

};

