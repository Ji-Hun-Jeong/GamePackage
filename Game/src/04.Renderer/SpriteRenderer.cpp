#include "pch.h"
#include "SpriteRenderer.h"

#include "RenderResourceLoader.h"
#include "PSOManager.h"

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

	RenderStates.resize(100, { 0 });

	ModelBuffer = CRenderResourceLoader::GetInst().CreateConstBuffer(sizeof(Matrix));
	ViewBuffer = CRenderResourceLoader::GetInst().CreateConstBuffer(sizeof(TViewData));

	
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
	ViewData.ScreenWidth = InScreenWidth;
	ViewData.ScreenHeight = InScreenHeight;

	Graphics::TViewPort ViewPort;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = static_cast<float>(InScreenWidth);
	ViewPort.Height = static_cast<float>(InScreenHeight);
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;

	Context.RSSetViewPort(&ViewPort);
}

void CSpriteRenderer::SetViewInfo(const Vector3& InWorldPosition, const Vector3& InRotation, const Vector3& InScale, bool bInUseScreenSize)
{
	RendererPosition = InWorldPosition;
	RendererRotation = InRotation;
	RendererScale = InScale;
	if (bInUseScreenSize)
		RendererScale *= Vector3(float(ViewData.ScreenWidth), float(ViewData.ScreenHeight), 1.0f);
}

bool CSpriteRenderer::IsInsideRenderer(const Vector3& InWorldPosition, const Vector3& InScale) const
{
	Vector3 RelativePosition = InWorldPosition - RendererPosition;

	float halfW = InScale.x * 0.5f;
	float halfH = InScale.y * 0.5f;

	float left = RelativePosition.x - halfW;
	float right = RelativePosition.x + halfW;
	float top = RelativePosition.y - halfH;
	float bottom = RelativePosition.y + halfH;

	float screenLeft = -float(ViewData.ScreenWidth) * 0.5f;
	float screenRight = float(ViewData.ScreenWidth) * 0.5f;
	float screenTop = -float(ViewData.ScreenHeight) * 0.5f;
	float screenBottom = float(ViewData.ScreenHeight) * 0.5f;

	bool isInside =
		right >= screenLeft &&
		left <= screenRight &&
		bottom >= screenTop &&
		top <= screenBottom;

	return isInside;
}

void CSpriteRenderer::TransformNDC(TRenderState* InOutRenderState) const
{
	assert(InOutRenderState);
	InOutRenderState->Scale *= Vector3(1.0f / ViewData.ScreenWidth, 1.0f / ViewData.ScreenHeight, 1.0);
	InOutRenderState->Position *= Vector3(1.0f / (ViewData.ScreenWidth * 0.5f), 1.0f / (ViewData.ScreenHeight * 0.5f), 1.0);
}
