#pragma once
#include "Component.h"
#include "07.Collision/PixelCollisionManager.h"

class CPixelCollider : public CComponent
{
	GENERATE_OBJECT(CPixelCollider)
public:
	CPixelCollider() = default;
	~CPixelCollider() = default;

public:
	// 일단 대충 쓸수만 있게
	void StartUse()
	{
		PixelCollisionObject = std::make_unique<CPixelCollisionObject>();
	}
	void SetPixelMap(EPixelMapType InPixelMapType, const std::wstring& InPixelMapPath)
	{
		PixelMapPaths[size_t(InPixelMapType)] = InPixelMapPath;
	}
	void SetPosition(const Vector3& InPosition)
	{
		if (PixelCollisionObject)
			PixelCollisionObject->SetPosition(Vector2(InPosition.x, InPosition.y));
	}
	void CollisionProcess(CPixelCollisionManager& InPixelCollisionManager)
	{
		for (size_t i = 0; i < PixelMapPaths.size(); ++i)
		{
			const auto& PixelMapPath = PixelMapPaths[i];
			if (PixelMapPath.empty())
				continue;
			InPixelCollisionManager.SetPixelMap(EPixelMapType(i), PixelMapPath);
		}

		if (PixelCollisionObject)
			InPixelCollisionManager.RequestCollisionCheck(*PixelCollisionObject.get());
	}

private:
	std::unique_ptr<CPixelCollisionObject> PixelCollisionObject;

	std::array<std::wstring, 6> PixelMapPaths;

};

