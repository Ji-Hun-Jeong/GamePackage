#include "pch.h"
#include "GroundManager.h"

#include "GameCore.h"

void CGroundManager::AddGround(const Vector3& InPosition, const Vector3& InScale)
{
	CGround* Ground = GetWorld()->SpawnActor<CGround>(this);

	CRectCollider* Collider = Ground->AddComponent<CRectCollider>();
	Collider->SetOffset(Vector2(InPosition.x, InPosition.y));
	Collider->SetRectScale(Vector2(InScale.x, InScale.y));
	Collider->SetDebugRender(true);

	Ground->RectCollider = Collider;

	Grounds.push_back(Ground);
}

