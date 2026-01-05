#pragma once
#include "01.Base/Actor/Actor.h"

class CGround : public CActor
{
	friend class CGroundManager;
	GENERATE_OBJECT(CGround)
public:
	CGround() = default;
	~CGround() = default;

public:
	virtual Vector3 GetSurfacePosition(const Vector3& InPosition) const
	{
		float SurfaceY = RectCollider->GetFinalPosition().y + RectCollider->GetRectScale().y / 2.0f;
		return Vector3(InPosition.x, SurfaceY, InPosition.z);
	}

private:
	CRectCollider* RectCollider = nullptr;

};
class CGroundManager : public CActor
{
	GENERATE_OBJECT(CGroundManager)
public:
	CGroundManager() = default;
	~CGroundManager() = default;

public:
	void AddGround(const Vector3& InPosition, const Vector3& InScale);

	bool RemoveProximateGround(const Vector2& InPosition)
	{
		for (auto Iter = Grounds.begin(); Iter != Grounds.end(); ++Iter)
		{
			CGround* Ground = *Iter;
			if (Ground->RectCollider->IsInside(InPosition))
			{
				Grounds.erase(Iter);
				Ground->Destroy();
				return true;
			}
		}
		return false;
	}

	void ClearGrounds()
	{
		for (CGround* Ground : Grounds)
			Ground->Destroy();
		Grounds.clear();
	}

private:
	std::vector<CGround*> Grounds;

};

