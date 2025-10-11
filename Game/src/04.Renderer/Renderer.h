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
		, SortFlag(false)
	{}
	~CRenderer() = default;

public:
	void RegistRenderStateObject(std::unique_ptr<CRenderStateObject> InRenderStateObject)
	{
		RenderStateObjects.push_back(std::move(InRenderStateObject));
		SortFlag = true;
	}
	void CleanUpRenderStateObjects()
	{
		for (size_t i = 0; i < RenderStateObjects.size();)
		{
			std::unique_ptr<CRenderStateObject>& RenderStateObject = RenderStateObjects[i];
			if (RenderStateObject->BeDestroy())
			{
				RenderStateObjects[i] = std::move(RenderStateObjects.back());
				RenderStateObjects.pop_back();
				SortFlag = true;
			}
			else
				i += 1;
		}
	}
	void SortRenderStateObjects()
	{
		if (SortFlag)
		{
			std::sort(RenderStateObjects.begin(), RenderStateObjects.end(),
				[](std::unique_ptr<CRenderStateObject>& InA, std::unique_ptr<CRenderStateObject>& InB)->bool
				{
					return *InA.get() > *InB.get();
				});
			SortFlag = false;
		}
	}
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

	std::vector<std::unique_ptr<CRenderStateObject>> RenderStateObjects;

	bool SortFlag;

};

