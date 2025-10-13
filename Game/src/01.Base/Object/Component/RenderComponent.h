#pragma once
#include "Component.h"
#include "04.Renderer/RenderStateObject.h"

class CRenderComponent : public CComponent
{
	GenerateObject()
public:
	CRenderComponent()
		: RenderStateObject(nullptr)
	{}
	~CRenderComponent() = default;

public:
	void SetRenderStateObject(CRenderStateObject* InRenderStateObject)
	{
		RenderStateObject = InRenderStateObject;
		SetRenderStateObjectEvent(RenderStateObject);
	}
	CRenderStateObject* GetRenderStateObject() const { return RenderStateObject; }
	std::function<void(CRenderStateObject*)> SetRenderStateObjectEvent;
private:
	CRenderStateObject* RenderStateObject;

};

