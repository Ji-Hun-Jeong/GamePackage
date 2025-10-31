#pragma once
#include "Actor.h"

struct TCameraConst
{
	Matrix ViewProj;
	uint32_t ScreenWidth = 0;
	uint32_t ScreenHeight = 0;
	uint32_t Dummy[2] = { 0 };
};
static_assert(sizeof(TCameraConst) % 16 == 0);

class CCamera : public CActor
{
	GENERATE_OBJECT(CCamera)
	DONTCOPY(CCamera)
public:
	CCamera();
	~CCamera() = default;

public:
	void CaptureSnapShot(uint32_t InScreenWidth, uint32_t InScreenHeight) override;
	void SetupInputActionValue(class CInputActionValueCollector& InInputActionValueCollector) override;

private:
	TCameraConst CameraConst;

	std::unique_ptr<class CInputActionValue> LeftMoveActionValue;
	std::unique_ptr<class CInputActionValue> RightMoveActionValue;
	std::unique_ptr<class CInputActionValue> UpMoveActionValue;
	std::unique_ptr<class CInputActionValue> DownMoveActionValue;
};

