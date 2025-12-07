#pragma once
#include "01.Base/Actor/Actor.h"

class CGroundManager : public CActor
{
	GENERATE_OBJECT(CGroundManager)
public:
	CGroundManager() = default;
	~CGroundManager() = default;

public:
	void AddGroundCollider(const Vector3& InPosition, const Vector3& InScale)
	{
		CRectCollider* Collider = AddComponent<CRectCollider>();
		Collider->SetOffset(Vector2(InPosition.x, InPosition.y));
		Collider->SetRectScale(Vector2(InScale.x, InScale.y));
		Collider->SetDebugRender(true);
	}

	void RemoveProximateCollider(const Vector2& InPosition)
	{
		for (auto Iter = Colliders.begin(); Iter != Colliders.end(); ++Iter)
		{
			CCollider* Collider = *Iter;
			if (Collider->IsInside(InPosition))
			{
				DetachComponent(Collider);
				break;
			}
		}
	}

private:


};

