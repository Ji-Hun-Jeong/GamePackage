#pragma once
#include "../Component.h"

enum class EColliderType
{
	Rect,
	Circle,
	None
};

class CCollider : public CComponent
{
public:
	CCollider(EColliderType InColliderType)
		: ColliderType(InColliderType)
	{}
	virtual ~CCollider() = default;

public:
	enum class ECollisionState
	{
		Enter,
		Stay,
		Exit
	};
	struct TCollisionInfo
	{
		CCollider* TargetCollider;
		ECollisionState CollisionState;
	};

public:
	void SetCenterPosition(const Vector2& InCenterPosition) { CenterPosition = InCenterPosition; }
	const Vector2& GetCenterPosition() const { return CenterPosition; }
	EColliderType GetColliderType() const { return ColliderType; }

	void ReadyToCollision()
	{
		CollisionInfos.clear();
	}

	void DeliverCollisionInfo(CCollider& InCollisionTarget, ECollisionState InCollisionState)
	{
		CollisionInfos.push_back({ &InCollisionTarget, InCollisionState });
	}

private:
	EColliderType ColliderType;

	std::vector<TCollisionInfo> CollisionInfos;

	Vector2 CenterPosition;

};

class CRectCollider : public CCollider
{
	GENERATE_OBJECT(CRectCollider)
public:
	CRectCollider()
		: CCollider(EColliderType::Rect)
	{}
	~CRectCollider() = default;

public:
	void SetRectScale(const Vector2& InRectScale) { RectScale = InRectScale; }
	const Vector2& GetRectScale() const { return RectScale; }

private:
	Vector2 RectScale;

};

class CCircleCollider : public CCollider
{
	GENERATE_OBJECT(CCircleCollider)
public:
	CCircleCollider()
		: CCollider(EColliderType::Circle)
		, Radius(0.0f)
	{}
	~CCircleCollider() = default;

public:
	void SetRadius(float InRadius) { Radius = InRadius; }
	float GetRadius() const { return Radius; }

private:
	float Radius;
};

