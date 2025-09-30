#pragma once
#include "../Object.h"

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
		// 자신 업뎃 하고 ActorComponent업데이트
	}

};

