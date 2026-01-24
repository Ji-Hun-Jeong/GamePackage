#include "pch.h"
#include "GroundDetector.h"

#include "GameCore.h"
#include "02.Contents/Actor/Player/Player.h"

CGroundDetector::CGroundDetector()
	: DetectCollider(AddComponent<CRectCollider>())
{
	DetectCollider->SetDebugRender(true);
	Activate(false);
}

void CGroundDetector::AdjustPlayerPosition(CPlayer& InPlayer)
{
	if (IsOnGround() == false)
		return;

	const Vector2 DetectCenterPosition = DetectCollider->GetFinalPosition();
	const Vector2 DetectScale = DetectCollider->GetRectScale();

	const Vector3& SurfacePosition = Ground->GetSurfacePosition(Vector3(DetectCenterPosition.x, DetectCenterPosition.y, 0.0f));

	const Vector3 NewDetectorPosition = SurfacePosition + Vector3(0.0f, DetectScale.y / 2.0f, 0.0f);
	const Vector3 NewPlayerPosition = NewDetectorPosition + Vector3(0.0f, std::abs(Transform->GetPosition().y), 0.0f);

	InPlayer.GetTransform()->SetPosition(NewPlayerPosition);
}
