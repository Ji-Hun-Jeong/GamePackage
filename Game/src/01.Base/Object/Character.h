#pragma once
#include "Actor.h"

class CCharacter : public CActor
{
	DONTCOPY(CCharacter)
public:
	CCharacter() = default;
	virtual ~CCharacter() = 0 {}
public:
	virtual void Initalize() override
	{
		CActor::Initalize();
		CActor::SetRenderComponent();
	}

private:

};

