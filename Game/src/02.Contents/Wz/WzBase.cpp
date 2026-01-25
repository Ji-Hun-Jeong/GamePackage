#include "pch.h"
#include "WzBase.h"

#include "01.Base/Actor/Actor.h"
#include "04.Renderer/RenderResourceLoader.h"
Vector3 CWzUtils::GetWorldPositionFromWzPosition(Vector2 InImageScale, Vector2 InWzOrigin)
{
	float FinalX = (InImageScale.x / 2.0f) - InWzOrigin.x;
	float FinalY = (InImageScale.y / 2.0f) - InWzOrigin.y;

	return Vector3(FinalX, -FinalY, 0.0f);
}

Vector3 CWzUtils::GetWorldPositionFromWzPosition(const std::wstring& InImagePath, Vector2 InWzOrigin)
{
	CImage* Image = CRenderResourceLoader::GetInst().GetImage(InImagePath);
	return GetWorldPositionFromWzPosition(Image->GetTexture2D().GetTextureSize(), InWzOrigin);
}

Vector3 CWzUtils::GetWorldPositionFromWzPosition(const CSpriteRenderComponent& InSpriteRenderComponent, Vector2 InWzOrigin)
{
	Vector2 ImageSize = InSpriteRenderComponent.GetImageScale();
	return GetWorldPositionFromWzPosition(ImageSize, InWzOrigin);
}

bool CWzUtils::SetWorldPositionFromWzPosition(CActor& InActor, Vector2 InWzOrigin)
{
	if (CSpriteRenderComponent* RC = InActor.GetSpriteRenderComponent())
	{
		Vector3 WorldPosition = GetWorldPositionFromWzPosition(*RC, InWzOrigin);
		InActor.GetTransform()->AddPositionOffset(WorldPosition);
		return true;
	}
	return false;
}
namespace Wz
{
	CWzNode* GenerateWzData(const JValue& InValue)
	{
		CWzNode* NewNode = new CWzNode();

		if (InValue.IsObject())
		{
			const auto& Object = InValue.GetObject();
			for (const auto& Member : Object)
			{
				const std::string_view Name = Member.name.GetString();

				CWzNode* ChildNode = GenerateWzData(Member.value);
				if (ChildNode)
					NewNode->AddMember(Name, *ChildNode);
			}
		}
		else if (InValue.IsArray())
		{
			// 배열인 경우 (예: [ {..}, {..} ]) 
			// 키를 "0", "1", "2" 문자열로 변환하여 자식으로 편입
			const auto& Array = InValue.GetArray();
			for (rapidjson::SizeType i = 0; i < Array.Size(); ++i)
			{
				CWzNode* ChildNode = GenerateWzData(Array[i]);
				if (ChildNode)
					NewNode->AddMember(std::to_string(i), *ChildNode);
			}
		}
		else
		{
			if (InValue.IsString())
				NewNode->SetValue(InValue.GetString());
		}

		return NewNode;
	}
}