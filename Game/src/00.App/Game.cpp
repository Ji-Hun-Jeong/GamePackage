#include "pch.h"
#include "Game.h"
#include <Renderer/Platform/DX/DXInfra.h>
#include "00.App/MouseManager.h"

class CSetMousePositionToMouseManager : public Core::IMouseMove
{
	void MouseMove(int InX, int InY, uint32_t InScreenWidth, uint32_t InScreenHeight) override
	{
		float MouseX = float(InX) - InScreenWidth * 0.5f;
		float MouseY = float(-InY) + InScreenHeight * 0.5f;
		CMouseManager::GetInst().SetMousePosition({ MouseX, MouseY });
	}
};
class CSetScreenSize : public Core::IWindowResize
{
	void WindowResize(UINT InNewScreenWidth, UINT InNewScreenHeight) override
	{
		CWindowManager::GetInst().SetScreenWidth(InNewScreenWidth);
		CWindowManager::GetInst().SetScreenHeight(InNewScreenHeight);
	}
};

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
	: Core::CApplication(InScreenWidth, InScreenHeight)
	, GraphicInfra(std::make_unique<Graphics::DX::CDXInfra>(Window.GetWindowHandle(), Window.GetScreenWidth(), Window.GetScreenHeight()))
	, SpriteRenderer(*GraphicInfra.get(), Window.GetScreenWidth(), Window.GetScreenHeight())
	, ImGuiManager(CImGuiManager::GetInst())
	, PixelCollisionManager(GraphicInfra->GetDevice(), CRenderResourceLoader::GetInst(), 1000)
	, MouseInteractionManager()
	, World()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr)) assert(0);

	CWindowManager::GetInst().SetScreenWidth(Window.GetScreenWidth());
	CWindowManager::GetInst().SetScreenHeight(Window.GetScreenHeight());

	Window.RegistMouseMoveEvent(std::make_unique<CSetMousePositionToMouseManager>());
	Window.RegistWindowResizeEvent(std::make_unique<CSetScreenSize>());

	World.Start();
}

CGame::~CGame()
{
	CoUninitialize();
}

bool CGame::Process()
{
	CCoreSystem::GetInst().ArrangeObjects();

	ImGuiManager.StartFrame("ImGui");

	Core::CInputManager::GetInst().Update();
	CMouseManager::GetInst().SetStateByInputManager(Core::CInputManager::GetInst());

	World.Arrange();
	World.Ready();

	World.Update();
	World.CaptureSnapShot();

	World.RenderWorld(SpriteRenderer);

	ImGuiManager.EndFrame();
	ImGuiManager.Render();

	SpriteRenderer.Present();
	return true;
}

void CGame::ShutDown()
{
	CCoreSystem::GetInst().ShutDown();
}