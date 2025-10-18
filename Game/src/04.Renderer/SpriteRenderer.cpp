#include "pch.h"
#include "SpriteRenderer.h"

#include <Core/public/Window.h>
#include "01.Base/World/World.h"

class CInitalizeRenderComponent : public INewObjectEvent
{
public:
	CInitalizeRenderComponent(CSpriteRenderer& InRenderer, CRenderResourceLoader& InRenderResourceLoader)
		: Renderer(InRenderer)
		, RenderResourceLoader(InRenderResourceLoader)
	{}
private:
	void CreatedInWorld(CWorld& InWorld, CObject& InNewObject) override
	{
		CRenderComponent* RenderComponent = static_cast<CRenderComponent*>(&InNewObject);
		CRenderStateObject* RenderStateObject = Renderer.NewRenderStateObject();

		RenderComponent->SetRenderStateObject(RenderStateObject);
		RenderComponent->SetRenderResourceLoader(&RenderResourceLoader);
		RenderComponent->SetMesh(0);
		RenderComponent->SetPSO(EPSOType::Basic);
	}

	CSpriteRenderer& Renderer;
	CRenderResourceLoader& RenderResourceLoader;
};
CSpriteRenderer::CSpriteRenderer(std::unique_ptr<Graphics::IGraphicInfra> InGraphicInfra, uint32_t InScreenWidth, uint32_t InScreenHeight)
	: ScreenWidth(InScreenWidth)
	, ScreenHeight(InScreenHeight)
	, GraphicInfra(std::move(InGraphicInfra))
	, Device(GraphicInfra->GetDevice())
	, Context(GraphicInfra->GetContext())
	, SwapChain(GraphicInfra->GetSwapChain())
	, RenderTargetView(Device.CreateRenderTargetView(*SwapChain.GetWindowTextureBuffer()))
	, PSOManager(Device)
	, RenderResourceLoader(Device, PSOManager, ScreenWidth, ScreenHeight)
{
	SetViewPort(ScreenWidth, ScreenHeight);
	Context.OMSetRenderTargets(1, RenderTargetView.get(), nullptr);

	TGeometryData GeometryData = CGeometryGenerator::GenerateSquare();

	Graphics::TMeshData MeshData;
	MeshData.VertexBufferDesc.BindFlags = Graphics::EBindFlags::BindVertexBuffer;
	MeshData.VertexBufferDesc.ByteWidth = uint32_t(sizeof(TVertex) * GeometryData.Vertices.size());
	MeshData.VertexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
	MeshData.VertexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;
	MeshData.VertexBufferInitData.CopyStartPoint = GeometryData.Vertices.data();

	MeshData.IndexBufferDesc.BindFlags = Graphics::EBindFlags::BindIndexBuffer;
	MeshData.IndexBufferDesc.ByteWidth = uint32_t(sizeof(uint32_t) * GeometryData.Indices.size());
	MeshData.IndexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
	MeshData.IndexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;
	MeshData.IndexBufferInitData.CopyStartPoint = GeometryData.Indices.data();

	MeshData.IndexFormat = Graphics::EGIFormat::GI_FORMAT_R32_UINT;
	MeshData.IndexCount = uint32_t(GeometryData.Indices.size());
	MeshData.Stride = sizeof(TVertex);
	MeshData.Offset = 0;
	MeshData.Key = 0;

	RenderResourceLoader.LoadMesh(MeshData);
}

void CSpriteRenderer::SetWindowSize(uint32_t InScreenWidth, uint32_t InScreenHeight)
{
	ScreenWidth = InScreenWidth;
	ScreenHeight = InScreenHeight;
	SetViewPort(ScreenWidth, ScreenHeight);

	Context.OMSetRenderTargets(0, nullptr, nullptr);
	RenderTargetView.reset();

	SwapChain.ResizeBuffers(0, ScreenWidth, ScreenHeight, Graphics::EGIFormat::GI_FORMAT_UNKNOWN);
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

	Context.RSSetViewPort(ViewPort);
}

void CSpriteRenderer::InitalizeFromWorld(CWorld& InWorld)
{
	InWorld.AddNewObjectTypeEvent(CRenderComponent::GetStaticType(), std::make_unique<CInitalizeRenderComponent>(*this, RenderResourceLoader));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CRefreshScreen : public Core::IWindowResize
{
public:
	CRefreshScreen(CSpriteRenderer& InSprireRenderer)
		: SpriteRenderer(InSprireRenderer)
	{}
	void WindowResize(UINT InNewScreenWidth, UINT InNewScreenHeight) override
	{
		SpriteRenderer.SetWindowSize(InNewScreenWidth, InNewScreenHeight);
	}
private:
	CSpriteRenderer& SpriteRenderer;
};

void CSpriteRenderer::InitalizeFromWindow(Core::CWindow& InWindow)
{
	InWindow.RegistWindowResizeEvent(std::make_unique<CRefreshScreen>(*this));
}