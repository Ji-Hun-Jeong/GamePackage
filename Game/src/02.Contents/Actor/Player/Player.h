#pragma once
#include "01.Base/Object/Character.h"

class CPlayer : public CCharacter
{
	GenerateObject()
	DONTCOPY(CPlayer)
public:
	CPlayer() = default;
	~CPlayer() = default;

public:
	void Initalize() override;
	void Update(float InDeltaTime) override;
private:

};

