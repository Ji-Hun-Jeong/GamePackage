#include "pch.h"
#include "WzLoader.h"
#include "01.Base/Actor/Actor.h"

Vector3 CWzUtils::GetWorldPositionFromOrigin(const CSpriteRenderComponent& InSpriteRenderComponent, const Vector2& InWzOrigin)
{
	// 1. 실제 로드된 이미지의 해상도(크기)를 가져옵니다.
// 예: 100x200 사이즈 이미지라면
	Vector2 ImageSize = InSpriteRenderComponent.GetImageScale();

	// 2. 엔진이 중앙 정렬을 하므로, 좌상단(0,0)을 맞추기 위해 
	// 이미지 크기의 절반만큼 더해준 뒤, WZ의 Origin만큼 뺍니다.
	float FinalX = (ImageSize.x / 2.0f) - InWzOrigin.x;
	float FinalY = (ImageSize.y / 2.0f) - InWzOrigin.y;

	// Y축이 엔진에서 위가 +라면, 메이플 좌표(아래가 +)와 맞추기 위해 
	// Y값에 마이너스를 붙여야 할 수도 있습니다. (엔진 좌표계 확인 필요)
	return Vector3(FinalX, -FinalY, 0.0f);
}

bool CWzUtils::SetWorldPositionFromOrigin(CActor& InActor, const Vector2& InWzOrigin)
{
	if (CSpriteRenderComponent* RC = InActor.GetSpriteRenderComponent())
	{
		Vector3 WorldPosition = GetWorldPositionFromOrigin(*RC, InWzOrigin);
		InActor.GetTransform()->SetPosition(WorldPosition);
		return true;
	}
	return false;
}