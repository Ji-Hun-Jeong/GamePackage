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
	Vector2 GetFinalPosition() const { return CenterPosition + Offset; }
	EColliderType GetColliderType() const { return ColliderType; }

	void ReadyToCollisionCheck()
	{
		while (CollisionInfos.empty() == false)
			CollisionInfos.pop();
	}

	void ReceiveCollisionInfo(CCollider& InCollisionTarget, ECollisionState InCollisionState)
	{
		CollisionInfos.push({ &InCollisionTarget, InCollisionState });
	}

	bool IsEmptyCollisionInfo() const { return CollisionInfos.empty(); }
	TCollisionInfo GetCollisionInfo()
	{
		TCollisionInfo CollisionInfo = CollisionInfos.front();
		CollisionInfos.pop();
		return CollisionInfo;
	}

private:
	EColliderType ColliderType;

	std::queue<TCollisionInfo> CollisionInfos;

	Vector2 CenterPosition;
	Vector2 Offset;

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

