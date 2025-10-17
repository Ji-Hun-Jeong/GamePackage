#pragma once
#include <Renderer/Base/GraphicInfra.h>
#include "RenderResourceLoader.h"
#include "RenderStateObject.h"

class CSpriteRenderer
{
public:
	CSpriteRenderer(std::unique_ptr<Graphics::IGraphicInfra> InGraphicInfra, const uint32_t& InScreenWidth, const uint32_t& InScreenHeight);
	~CSpriteRenderer() = default;

public:
	void InitalizeFromWorld(class CWorld& InWorld);
	CRenderStateObject* NewRenderStateObject()
	{
		CRenderStateObject* RenderStateObject = new CRenderStateObject;
		RenderStateObjects.emplace_back(RenderStateObject);

		return RenderStateObject;
	}
	void Render()
	{
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Context.ClearRenderTarget(*RenderTargetView, ClearColor);

		// 만약에 순서가 문제가 되면 그때가서 DestroyEvent 넣기
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
	const uint32_t& ScreenWidth;
	const uint32_t& ScreenHeight;

	std::unique_ptr<Graphics::IGraphicInfra> GraphicInfra;
	Graphics::CRenderDevice& Device;
	Graphics::CRenderContext& Context;
	Graphics::CRenderSwapChain& SwapChain;

	CPSOManager PSOManager;
	CRenderResourceLoader RenderResourceLoader;

	std::unique_ptr<Graphics::CRenderTargetView> RenderTargetView;

	std::vector<std::unique_ptr<CRenderStateObject>> RenderStateObjects;



};