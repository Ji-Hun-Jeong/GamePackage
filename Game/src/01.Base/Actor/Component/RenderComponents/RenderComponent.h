#pragma once
#include "../Component.h"
#include "04.Renderer/RenderStateObject.h"

class CRenderComponent : public CComponent
{
public:
	CRenderComponent() = default;
	virtual ~CRenderComponent() = 0 {}

public:
	virtual void Render(class CSpriteRenderer& InRenderer) = 0;
public:
	/*void Serialize(CSerializer& InSerializer) const override
	{
		CComponent::Serialize(InSerializer);
		if (ImageDescs[0].IsValid())
		{
			CSerializer Data;
			Data["ImagePath"] = ImageDescs[0].GetImagePath().c_str();
			InSerializer = Data;
		}
	}
	void Deserialize(const CSerializer& InDeserializer) override
	{
		CComponent::Deserialize(InDeserializer);
		const std::string& Path = InDeserializer["ImagePath"];
		SetDiffuseImage(std::wstring(Path.begin(), Path.end()));
	}*/

protected:
	CRenderStateObject RenderStateObject;

};

