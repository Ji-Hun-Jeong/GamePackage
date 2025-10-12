#include "pch.h"
#include "Game.h"

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
	: Core::CApplication(InScreenWidth, InScreenHeight)
	, World()
	, GraphicInfra(Window.GetWindowHandle())
	, Device(GraphicInfra.GetDevice())
	, Context(GraphicInfra.GetContext())
	, SwapChain(GraphicInfra.GetSwapChain())
	, Renderer(Context, SwapChain)
{
	Renderer.InitalizeFromWorld(World);
	
	CAssetLoader::GetInst().Initalize(&Device, &MeshManager);

	Graphics::TRasterizerDesc RasterizerDesc;
	RasterizerDesc.FillMode = Graphics::EFillMode::FillSolid;
	RasterizerDesc.CullMode = Graphics::ECullMode::CullBack;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.MultisampleEnable = true;
	auto RasterizerState = Device.CreateRasterizerState(RasterizerDesc);

	const Graphics::CTexture2D* WindowTextureBuffer = SwapChain.GetWindowTextureBuffer();
	auto RenderTargetView = Device.CreateRenderTargetView(*WindowTextureBuffer);

	Graphics::TViewPort ViewPort;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = static_cast<float>(1280);
	ViewPort.Height = static_cast<float>(960);
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;

	Context.RSSetViewPort(ViewPort);
	Context.OMSetRenderTarget(1, *RenderTargetView.get(), nullptr);
	Context.IASetPrimitiveTopology(Graphics::ETopology::PrimitiveTopologyTRIANGLELIST);

	Context.RSSetState(*RasterizerState.get());

}

CGame::~CGame()
{
}

bool CGame::Process()
{
	World.Update();
	Renderer.Render();
	return true;
}

void CGame::ShutDown()
{
}