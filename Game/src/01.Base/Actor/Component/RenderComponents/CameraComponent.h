#pragma once
#include "RenderComponent.h"

struct TCameraConst
{
	Matrix ViewProj;
	uint32_t ScreenWidth = 0;
	uint32_t ScreenHeight = 0;
	uint32_t Dummy[2] = { 0 };
};
static_assert(sizeof(TCameraConst) % 16 == 0);

class CCameraComponent : public CRenderComponent
{
	GENERATE_OBJECT(CCameraComponent)
public:
	CCameraComponent();
	~CCameraComponent() = default;

public:
	void Render(CSpriteRenderer& InRenderer) override;

	void UpdateViewToNDC(const Vector3& InPosition, const Vector3& InRotation, const Vector3& InScale
		, uint32_t InScreenWidth, uint32_t InScreenHeight)
	{
		Vector3 NormalizedScale = Vector3(
			InScale.x / InScreenWidth,
			InScale.y / InScreenHeight,
			InScale.z // Z축은 보통 그대로 둠
		);
		float NormalizedX = (InPosition.x / (InScreenWidth * 0.5f));
		float NormalizedY = (InPosition.y / (InScreenHeight * 0.5f));

		CameraConst.ViewProj= Matrix::CreateScale(NormalizedScale)
			* Matrix::CreateRotationX(InRotation.x)
			* Matrix::CreateRotationY(InRotation.y)
			* Matrix::CreateRotationZ(InRotation.z)
			* Matrix::CreateTranslation(Vector3(NormalizedX, NormalizedY, InPosition.z)).Invert().Transpose();
		CameraConst.ScreenWidth = InScreenWidth;
		CameraConst.ScreenHeight = InScreenHeight;

		bUpdateCameraConst = true;
	}

private:
	TCameraConst CameraConst;
	bool bUpdateCameraConst;

};

