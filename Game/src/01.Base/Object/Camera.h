#pragma once
#include "Actor.h"

struct TCameraConst
{
	Matrix ViewProj;
	uint32_t ScreenWidth;
	uint32_t ScreenHeight;
	uint32_t Dummy[2];
};
static_assert(sizeof(TCameraConst) % 16 == 0);

class CCamera : public CActor
{
	GENERATE_OBJECT()
	DONTCOPY(CCamera)
public:
	CCamera()
		: LeftMoveActionValue(nullptr)
		, RightMoveActionValue(nullptr)
		, UpMoveActionValue(nullptr)
		, DownMoveActionValue(nullptr)
	{}
	~CCamera();

public:
	void Initalize() override;
	void CaptureSnapShot() override;
	void SetInputAction(class CInputActionManager& InInputActionManager) override;

	void SetScreenSize(uint32_t InScreenWidth, uint32_t InScreenHeight)
	{
		CameraConst.ScreenWidth = InScreenWidth;
		CameraConst.ScreenHeight = InScreenHeight;
	}

private:
	TCameraConst CameraConst;

	class CInputActionValue* LeftMoveActionValue;
	class CInputActionValue* RightMoveActionValue;
	class CInputActionValue* UpMoveActionValue;
	class CInputActionValue* DownMoveActionValue;

};

