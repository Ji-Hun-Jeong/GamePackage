#pragma once
#include "StaticActor.h"

class CCharacter : public CStaticActor
{
	GENERATE_OBJECT(CCharacter)
	DONTCOPY(CCharacter)
public:
	CCharacter()
	{
		Animator = AddComponent<CAnimator>();
	}
	virtual ~CCharacter() {}
public:
	//virtual void Initalize() override
	//{
	//	CActor::Initalize();
	//	//CActor::SetRenderComponent();
	//}

private:

};

