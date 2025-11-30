#pragma once
#include "../Component.h"
#include "04.Renderer/RenderStateObject.h"

enum class EColliderType
{
	Rect,
	Circle,
	None
};

class CCollider : public CComponent
{
public:
	CCollider(EColliderType InColliderType);
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
	virtual void DebugRender(class CSpriteRenderer& InRenderer) {}
	void SetDebugRender(bool bInDebugRender) { bDebugRender = bInDebugRender; }

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

protected:
	CRenderStateObject RenderStateObject;
	bool bDebugRender;

};

class CRectCollider : public CCollider
{
	GENERATE_OBJECT(CRectCollider)
public:
	CRectCollider();
	~CRectCollider() = default;

public:
	void DebugRender(class CSpriteRenderer& InRenderer) override;
	void SetRectScale(const Vector2& InRectScale) { RectScale = InRectScale; }
	const Vector2& GetRectScale() const { return RectScale; }

private:
	Vector2 RectScale;

};

class CCircleCollider : public CCollider
{
	GENERATE_OBJECT(CCircleCollider)
public:
	CCircleCollider();
	~CCircleCollider() = default;

public:
	void SetRadius(float InRadius) { Radius = InRadius; }
	float GetRadius() const { return Radius; }

private:
	float Radius;
};

