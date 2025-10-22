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
		NextAddedRSO.emplace(RenderStateObject);

		return RenderStateObject;
	}
	void Render()
	{
		// 3. 한 번에 제거 (O(n))
		auto NewEnd = std::remove_if(RenderStateObjects.begin(), RenderStateObjects.end(),
			[](const auto& InRenderStateObject) { return InRenderStateObject->bDestroy; });
		RenderStateObjects.erase(NewEnd, RenderStateObjects.end());

		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Context.ClearRenderTarget(RenderTargetView.get(), ClearColor);

		for (auto& RenderStateObject : RenderStateObjects)
		{
			RenderStateObject->BindRenderState(Context);
		}

		SwapChain.Present();

		while (NextAddedRSO.empty() == false)
		{
			RenderStateObjects.push_back(std::move(NextAddedRSO.front()));
			NextAddedRSO.pop();
		}
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
	std::queue<std::unique_ptr<CRenderStateObject>> NextAddedRSO;

};