#pragma once
#include "Component.h"
#include "04.Renderer/RenderStateObject.h"

class CRenderComponent : public CComponent
{
	GenerateComponent()
public:
	CRenderComponent(CRenderStateObject* InRenderStateObject)
		: RenderStateObject(InRenderStateObject)
	{}
	~CRenderComponent() = default;

public:
	void SetRenderStateObject(CRenderStateObject* InRenderStateObject) { RenderStateObject = InRenderStateObject; }
	CRenderStateObject* GetRenderStateObject() const { return RenderStateObject; }

private:
	CRenderStateObject* RenderStateObject;

};

