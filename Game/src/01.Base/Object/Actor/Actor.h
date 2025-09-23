#pragma once
#include "ActorComponent.h"

class CActor : public CObject
{
	DONTASSN(CActor)
public:
	CActor() {}
	virtual ~CActor() {}

protected:
	CActor(const CActor& InOther) : CObject(InOther) {}
	CActor(CActor&& InOther) noexcept
		: CObject(std::move(InOther)) 
	{}

public:
	virtual void Update(float InDeltaTime)
	{
		CObject::Update(InDeltaTime);
	}

private:


};

