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
	CCollider(EColliderType InColliderType);
	virtual ~CCollider() = default;

public:
	virtual void DebugRender(class CSpriteRenderer& InRenderer) {}
	virtual bool IsInside(const Vector2& InPosition) = 0;
	void SetDebugRender(bool bInDebugRender) { bDebugRender = bInDebugRender; }

	void SetOffset(const Vector2& InOffset) { Offset = InOffset; }
	void SetCenterPosition(const Vector2& InCenterPosition) { CenterPosition = InCenterPosition; }
	Vector2 GetFinalPosition() const { return CenterPosition + Offset; }
	EColliderType GetColliderType() const { return ColliderType; }

private:
	EColliderType ColliderType;

	Vector2 CenterPosition;
	Vector2 Offset;

protected:
	bool bDebugRender;
	Vector3 Color = Vector3(1.0f, 0.0f, 0.0f);
	uint32_t Layer = 3;

};

class CRectCollider : public CCollider
{
	GENERATE_OBJECT(CRectCollider)
public:
	CRectCollider();
	~CRectCollider() = default;

public:
	void DebugRender(class CSpriteRenderer& InRenderer) override;
	bool IsInside(const Vector2& InPosition) override;
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
	bool IsInside(const Vector2& InPosition) override;
	void SetRadius(float InRadius) { Radius = InRadius; }
	float GetRadius() const { return Radius; }

private:
	float Radius;
};

