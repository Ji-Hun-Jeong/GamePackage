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
	void BeginPlay() override;
	void FinalUpdate() override;
	void CaptureSnapShot() override;
	void SetInputAction(class CInputActionManager& InInputActionManager) override;
	void SetScreenSize(uint32_t InScreenWidth, uint32_t InScreenHeight)
	{
		CameraConst.ScreenWidth = InScreenWidth;
		CameraConst.ScreenHeight = InScreenHeight;
	}
	void SetTransformEvent(std::function<void(const Vector2&)> TransformEvent) { TransformEvents.push_back(TransformEvent); }
	void Destroy() override;

private:
	TCameraConst CameraConst;

	class CInputActionValue* LeftMoveActionValue;
	class CInputActionValue* RightMoveActionValue;
	class CInputActionValue* UpMoveActionValue;
	class CInputActionValue* DownMoveActionValue;

	std::vector<std::function<void(const Vector2&)>> TransformEvents;

};

