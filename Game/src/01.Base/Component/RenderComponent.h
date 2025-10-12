#pragma once
#include "Component.h"
#include "04.Renderer/RenderStateObject.h"

class CRenderComponent : public CComponent
{
	GenerateComponent()
public:
	CRenderComponent(std::unique_ptr<CRenderStateObject> InRenderStateObject)
		: RenderStateObject(std::move(InRenderStateObject))
	{}
	~CRenderComponent() = default;

public:
	void SetRenderStateObject(std::unique_ptr<CRenderStateObject> InRenderStateObject) { RenderStateObject = std::move(InRenderStateObject); }
	CRenderStateObject* GetRenderStateObject() const { return RenderStateObject.get(); }

private:
	std::unique_ptr<CRenderStateObject> RenderStateObject;

};

