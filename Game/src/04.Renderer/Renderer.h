#pragma once
#include <Renderer/Base/RenderContext.h>
#include <Renderer/Base/RenderSwapChain.h>
#include "RenderStateObject.h"

// Todo: 렌더러가 World에서 가져오는 방식으로 변경
class CRenderer
{
public:
	CRenderer(Graphics::CRenderContext& InRenderContext, Graphics::CRenderSwapChain& InSwapChain)
		: Context(InRenderContext)
		, SwapChain(InSwapChain)
	{}
	~CRenderer() = default;

public:
	void GetRenderStateObjectFromWorld(class CWorld& InWorld);
	void Render()
	{
		for (auto& RenderStateObject : RenderStateObjects)
		{
			RenderStateObject->BindRenderState(Context);
		}
		SwapChain.Present();
	}

private:
	Graphics::CRenderContext& Context;
	Graphics::CRenderSwapChain& SwapChain;

	std::vector<CRenderStateObject*> RenderStateObjects;

};

