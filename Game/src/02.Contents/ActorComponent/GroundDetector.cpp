#include "pch.h"
#include "GroundDetector.h"

#include "GameCore.h"
#include "02.Contents/Actor/Player/Player.h"

CGroundDetector::CGroundDetector()
	: DetectCollider(AddComponent<CRectCollider>())
{
	DetectCollider->SetDebugRender(true);
}

void CGroundDetector::AdjustPlayerPosition(CPlayer& InPlayer)
{
	if (IsOnGround() == false)
		return;

	const Vector2 DetectCenterPosition = DetectCollider->GetFinalPosition();
	const Vector2 DetectScale = DetectCollider->GetRectScale();

	const Vector3& SurfacePosition = Ground->GetSurfacePosition(Vector3(DetectCenterPosition.x, DetectCenterPosition.y, 0.0f));

	const Vector2 UpDistance = Vector2(SurfacePosition.x, SurfacePosition.y) - DetectCenterPosition;

	const Vector3& PlayerPosition = InPlayer.GetTransform()->GetFinalPosition();
	InPlayer.GetTransform()->SetPosition(PlayerPosition + Vector3(UpDistance.x, UpDistance.y, 0.0f));
}
