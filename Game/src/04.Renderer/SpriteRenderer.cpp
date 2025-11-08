#include "pch.h"
#include "SpriteRenderer.h"

CSpriteRenderer::CSpriteRenderer(Graphics::IGraphicInfra& InGraphicInfra, uint32_t InScreenWidth, uint32_t InScreenHeight)
	: Device(InGraphicInfra.GetDevice())
	, Context(InGraphicInfra.GetContext())
	, SwapChain(InGraphicInfra.GetSwapChain())
	, RenderTargetView(Device.CreateRenderTargetView(*SwapChain.GetWindowTextureBuffer()))
	, PSOManager(Device)
	, RenderResourceLoader(Device)
{
	SetViewPort(InScreenWidth, InScreenHeight);
	Context.OMSetRenderTargets(1, RenderTargetView.get(), nullptr);
}

void CSpriteRenderer::SetWindowSize(uint32_t InScreenWidth, uint32_t InScreenHeight)
{
	SetViewPort(InScreenWidth, InScreenHeight);

	Context.OMSetRenderTargets(0, nullptr, nullptr);
	RenderTargetView.reset();

	SwapChain.ResizeBuffers(0, InScreenWidth, InScreenHeight, Graphics::EGIFormat::GI_FORMAT_UNKNOWN);
	SwapChain.GenerateWindowTextureBuffer();

	RenderTargetView = Device.CreateRenderTargetView(*SwapChain.GetWindowTextureBuffer());

	Context.OMSetRenderTargets(1, RenderTargetView.get(), nullptr);
}

void CSpriteRenderer::SetViewPort(uint32_t InScreenWidth, uint32_t InScreenHeight)
{
	Graphics::TViewPort ViewPort;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = static_cast<float>(InScreenWidth);
	ViewPort.Height = static_cast<float>(InScreenHeight);
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;

	Context.RSSetViewPort(&ViewPort);
}