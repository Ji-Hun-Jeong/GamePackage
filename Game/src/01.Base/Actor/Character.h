#pragma once
#include "Actor.h"

class CCharacter : public CActor
{
	GENERATE_OBJECT(CCharacter)
	DONTCOPY(CCharacter)
public:
	CCharacter() = default;
	virtual ~CCharacter() {}
public:
	virtual void Initalize() override
	{
		CActor::Initalize();
		//CActor::SetRenderComponent();
	}

private:

};

