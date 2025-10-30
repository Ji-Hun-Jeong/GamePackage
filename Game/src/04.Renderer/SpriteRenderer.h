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
	void InitalizeFromWindow(Core::CWindow& InWindow);
	void SetWindowSize(uint32_t InScreenWidth, uint32_t InScreenHeight);
	void SetViewPort(uint32_t InScreenWidth, uint32_t InScreenHeight);
	uint32_t GetScreenWidth() const { return ScreenWidth; }
	uint32_t GetScreenHeight() const { return ScreenHeight; }
	void UpdateRSOs(std::queue<TBufferMappingInstance>& InBufferMappingInstances)
	{
		while (InBufferMappingInstances.empty() == false)
		{
			TBufferMappingInstance BufferMappingInstance = InBufferMappingInstances.front();
			InBufferMappingInstances.pop();

			BufferMappingInstance.RenderStateObject.UpdateVertexConstBuffer(Context, BufferMappingInstance.UpdateSlot
				, BufferMappingInstance.BufferData.data(), BufferMappingInstance.BufferData.size());
		}
	}
	void RenderRSOs(const std::vector<CRenderStateObject*>& InRenderStateObjects)
	{
		static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Context.ClearRenderTarget(RenderTargetView.get(), ClearColor);

		for (auto& RenderStateObject : InRenderStateObjects)
			RenderStateObject->BindRenderState(Context);

		SwapChain.Present();
	}

	const CPSOManager& GetPSOManager() const { return PSOManager; }
	CRenderResourceLoader& GetRenderResourceLoader() { return RenderResourceLoader; }

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

};