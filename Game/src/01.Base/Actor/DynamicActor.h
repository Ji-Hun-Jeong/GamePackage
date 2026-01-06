#pragma once
#include "StaticActor.h"
#include "01.Base/Actor/Component/Animation/Animator.h"

class CDynamicActor : public CStaticActor
{
	GENERATE_OBJECT(CDynamicActor)
		DONTCOPY(CDynamicActor)
public:
	CDynamicActor();
	virtual ~CDynamicActor() {}
public:
	void Update(float InDeltaTime) override
	{
		CStaticActor::Update(InDeltaTime);

	}
};

