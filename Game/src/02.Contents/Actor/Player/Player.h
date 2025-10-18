#pragma once
#include "01.Base/Object/Character.h"

class CPlayer : public CCharacter
{
	GENERATE_OBJECT()
	DONTCOPY(CPlayer)
public:
	CPlayer()
		: LeftMoveActionValue(nullptr)
		, RightMoveActionValue(nullptr)
		, UpMoveActionValue(nullptr)
		, DownMoveActionValue(nullptr)
	{}
	~CPlayer();

public:
	void Initalize() override;
	void Update(float InDeltaTime) override;
	void Destroy() override;
	void SetInputAction(class CInputActionManager& InInputActionManager);

private:
	class CInputActionValue* LeftMoveActionValue;
	class CInputActionValue* RightMoveActionValue;
	class CInputActionValue* UpMoveActionValue;
	class CInputActionValue* DownMoveActionValue;
	
};

