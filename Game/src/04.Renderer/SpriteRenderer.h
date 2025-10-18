#pragma once
#include <Renderer/Base/GraphicInfra.h>
#include "RenderResourceLoader.h"
#include "RenderStateObject.h"

namespace Core
{
	class CWindow;
}
class CSpriteRenderer
{
public:
	CSpriteRenderer(std::unique_ptr<Graphics::IGraphicInfra> InGraphicInfra, uint32_t InScreenWidth, uint32_t InScreenHeight);
	~CSpriteRenderer() = default;

public:
	void InitalizeFromWorld(class CWorld& InWorld);
	void InitalizeFromWindow(Core::CWindow& InWindow);
	void SetWindowSize(uint32_t InScreenWidth, uint32_t InScreenHeight);
	void SetViewPort(uint32_t InScreenWidth, uint32_t InScreenHeight);
	uint32_t GetScreenWidth() const { return ScreenWidth; }
	uint32_t GetScreenHeight() const { return ScreenHeight; }
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
	uint32_t ScreenWidth;
	uint32_t ScreenHeight;

	std::unique_ptr<Graphics::IGraphicInfra> GraphicInfra;
	Graphics::CRenderDevice& Device;
	Graphics::CRenderContext& Context;
	Graphics::CRenderSwapChain& SwapChain;

	CPSOManager PSOManager;
	CRenderResourceLoader RenderResourceLoader;

	std::unique_ptr<Graphics::CRenderTargetView> RenderTargetView;

	std::vector<std::unique_ptr<CRenderStateObject>> RenderStateObjects;



};