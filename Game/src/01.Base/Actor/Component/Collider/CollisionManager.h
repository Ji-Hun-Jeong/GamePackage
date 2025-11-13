#pragma once
#include "Collider.h"

union UCollisionKey
{
	struct
	{
		UINT ColliderId1;
		UINT ColliderId2;
	} BlendKey;
	size_t RealKey;
};
class CCollisionManager
{
public:
	CCollisionManager() = default;
	~CCollisionManager() = default;

public:
	void RequestCollision(CCollider& InCollider)
	{
		InCollider.ReadyToCollision();
		Colliders.push_back(&InCollider);
	}
	void CheckCollisionProcess()
	{
		for (size_t i = 0; i < Colliders.size(); ++i)
		{
			CCollider& Collider1 = *Colliders[i];
			for (size_t j = i + 1; j < Colliders.size(); ++j)
			{
				CCollider& Collider2 = *Colliders[j];

				bool bCollision = CheckCollision(Collider1, Collider2);
				bool bPrevCollision = IsPrevCollision(Collider1, Collider2);
				
				if (bPrevCollision)
				{
					if (Collider1.IsDestroy() || Collider2.IsDestroy())
					{
						// Exit
						Collider1.DeliverCollisionInfo(Collider2, CCollider::ECollisionState::Exit);
						Collider2.DeliverCollisionInfo(Collider1, CCollider::ECollisionState::Exit);
						UnRegistCollisionState(Collider1, Collider2);
						continue;
					}
				}

				if (bCollision)
				{
					if (bPrevCollision)
					{
						// Stay
						Collider1.DeliverCollisionInfo(Collider2, CCollider::ECollisionState::Stay);
						Collider2.DeliverCollisionInfo(Collider1, CCollider::ECollisionState::Stay);
					}
					else
					{
						// Enter
						Collider1.DeliverCollisionInfo(Collider2, CCollider::ECollisionState::Enter);
						Collider2.DeliverCollisionInfo(Collider1, CCollider::ECollisionState::Enter);
						RegistCollisionState(Collider1, Collider2);
					}
				}
				else
				{
					if (bPrevCollision)
					{
						// Exit
						Collider1.DeliverCollisionInfo(Collider2, CCollider::ECollisionState::Exit);
						Collider2.DeliverCollisionInfo(Collider1, CCollider::ECollisionState::Exit);
						UnRegistCollisionState(Collider1, Collider2);
					}
				}
			}
		}

		Colliders.clear();
	}

private:
	bool CheckCollision(CCollider& InCollider1, CCollider& InCollider2)
	{
		CCollider* Collider1 = &InCollider1;
		CCollider* Collider2 = &InCollider2;
		if (InCollider2.GetColliderType() < InCollider1.GetColliderType())
		{
			Collider1 = &InCollider2;
			Collider2 = &InCollider1;
		}
		
		if (Collider1->GetColliderType() == EColliderType::Rect && Collider2->GetColliderType() == EColliderType::Rect)
			return IsCollision(static_cast<CRectCollider&>(*Collider1), static_cast<CRectCollider&>(*Collider2));
		else if (Collider1->GetColliderType() == EColliderType::Rect && Collider2->GetColliderType() == EColliderType::Circle)
			return IsCollision(static_cast<CRectCollider&>(*Collider1), static_cast<CCircleCollider&>(*Collider2));
		else if (Collider1->GetColliderType() == EColliderType::Circle && Collider2->GetColliderType() == EColliderType::Circle)
			return IsCollision(static_cast<CCircleCollider&>(*Collider1), static_cast<CCircleCollider&>(*Collider2));
		else
			assert(0);
		return false;
	}
	bool IsPrevCollision(CCollider& InCollider1, CCollider& InCollider2)
	{
		size_t CollisionKey = GetCollisionKey(InCollider1, InCollider2);
		return CollisionStates.contains(CollisionKey);
	}
	bool IsCollision(CRectCollider& InRectCollider1, CRectCollider& InRectCollider2);
	bool IsCollision(CRectCollider& InRectCollider, CCircleCollider& InCircleCollider);
	bool IsCollision(CCircleCollider& InCircleCollider1, CCircleCollider& InCircleCollider2);
	void RegistCollisionState(CCollider& InCollider1, CCollider& InCollider2)
	{
		size_t CollisionKey = GetCollisionKey(InCollider1, InCollider2);
		CollisionStates.insert(CollisionKey);
	}
	void UnRegistCollisionState(CCollider& InCollider1, CCollider& InCollider2)
	{
		size_t CollisionKey = GetCollisionKey(InCollider1, InCollider2);
		CollisionStates.erase(CollisionKey);
	}
	size_t GetCollisionKey(CCollider& InCollider1, CCollider& InCollider2)
	{
		UINT InstanceId1 = InCollider1.GetInstanceId();
		UINT InstanceId2 = InCollider2.GetInstanceId();
		UINT ColliderId1 = 0;
		UINT ColliderId2 = 0;
		if (InstanceId1 < InstanceId2)
		{
			ColliderId1 = InstanceId1;
			ColliderId2 = InstanceId2;
		}
		else if (InstanceId2 < InstanceId1)
		{
			ColliderId2 = InstanceId1;
			ColliderId1 = InstanceId2;
		}
		else
		{
			// InstanceId가 같을 수가 없음
			assert(0);
		}
		UCollisionKey CollisionKey;
		CollisionKey.BlendKey.ColliderId1 = ColliderId1;
		CollisionKey.BlendKey.ColliderId2 = ColliderId2;
		return CollisionKey.RealKey;
	}

private:
	std::vector<CCollider*> Colliders;

	std::set<size_t> CollisionStates;

};

