#pragma once
#include <Renderer/Base/RenderContext.h>
#include <Renderer/Base/RenderSwapChain.h>
#include "RenderStateObject.h"

class CRenderer
{
public:
	CRenderer(Graphics::CRenderContext& InRenderContext, Graphics::CRenderSwapChain& InSwapChain)
		: Context(InRenderContext)
		, SwapChain(InSwapChain)
	{}
	~CRenderer() = default;

public:
	void InitalizeFromWorld(class CWorld& InWorld);
	void AddRenderStateObject(std::unique_ptr<CRenderStateObject> InRenderStateObject)
	{
		RenderStateObjects.push_back(std::move(InRenderStateObject));
	}
	void Render()
	{
		for (size_t i = 0; i < RenderStateObjects.size();)
		{
			auto& RenderStateObject = RenderStateObjects[i];
			if (RenderStateObject->bDestroy)
			{
				RenderStateObjects[i] = std::move(RenderStateObjects.back());
				RenderStateObjects.pop_back();
			}
			else
			{
				RenderStateObject->BindRenderState(Context);
				++i;
			}
		}

		SwapChain.Present();
	}

private:
	Graphics::CRenderContext& Context;
	Graphics::CRenderSwapChain& SwapChain;

	std::vector<std::unique_ptr<CRenderStateObject>> RenderStateObjects;

};

