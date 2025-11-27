#include "pch.h"
#include "SpriteRenderer.h"

#include "RenderResourceLoader.h"
#include "PSOManager.h"


CCulling::CCulling(Graphics::IGraphicInfra& InGraphicInfra)
	: Context(InGraphicInfra.GetContext())
{
}

CSpriteRenderer::CSpriteRenderer(Graphics::IGraphicInfra& InGraphicInfra, uint32_t InScreenWidth, uint32_t InScreenHeight)
	: Device(InGraphicInfra.GetDevice())
	, Context(InGraphicInfra.GetContext())
	, SwapChain(InGraphicInfra.GetSwapChain())
	, RenderTargetView(Device.CreateRenderTargetView(*SwapChain.GetWindowTextureBuffer()))
{
	CRenderResourceLoader::GetInst().Initalize(Device);
	CPSOManager::GetInst().Initalize(Device);

	SetViewPort(InScreenWidth, InScreenHeight);
	Context.OMSetRenderTargets(1, RenderTargetView.get(), nullptr);
}

CSpriteRenderer::~CSpriteRenderer()
{
	CRenderResourceLoader::GetInst().Finalize();
	CPSOManager::GetInst().Finalize();
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
	ScreenWidth = InScreenWidth;
	ScreenHeight = InScreenHeight;

	Graphics::TViewPort ViewPort;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = static_cast<float>(ScreenWidth);
	ViewPort.Height = static_cast<float>(ScreenHeight);
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;

	Context.RSSetViewPort(&ViewPort);
}

bool CSpriteRenderer::IsInsideNDC(const Vector2& InNDCPosition, const Vector2& InNDCScale)
{
	// NDC 좌표는 이미 -1.0 ~ 1.0 범위라고 가정
	Vector2 Position = InNDCPosition - CameraOffset;

	// 스케일도 NDC 단위로 들어온다고 가정 (예: 화면 크기 대비 비율)
	float halfW = InNDCScale.x * 0.5f;
	float halfH = InNDCScale.y * 0.5f;

	// 사각형 경계 (NDC 기준)
	float left = Position.x - halfW;
	float right = Position.x + halfW;
	float top = Position.y - halfH;
	float bottom = Position.y + halfH;

	// 화면 경계 (NDC는 -1 ~ 1)
	float screenLeft = -1.0f;
	float screenRight = 1.0f;
	float screenTop = -1.0f;
	float screenBottom = 1.0f;

	// 사각형이 화면 안에 있는지 확인 (겹치기만 해도 true)
	bool isInside =
		right >= screenLeft &&
		left <= screenRight &&
		bottom >= screenTop &&
		top <= screenBottom;

	return isInside;
}