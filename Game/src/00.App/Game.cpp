#include "pch.h"
#include "Game.h"
#include "04.Renderer/PSOManager.h"
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
	
	CPSOManager::GetInst().InitalizePSO(Device);
	CAssetLoader::GetInst().Initalize(&Device, &MeshManager);


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
	CAssetLoader::GetInst().Finalize();
	CPSOManager::GetInst().Finalize();
}