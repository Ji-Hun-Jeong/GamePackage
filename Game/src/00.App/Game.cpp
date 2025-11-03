#include "pch.h"
#include "Game.h"
#include <Renderer/Platform/DX/DXInfra.h>
#include "06.Interaction/MouseManager.h"

class CSetMousePositionToMouseManager : public Core::IMouseMove
{
	void MouseMove(int InX, int InY, uint32_t InScreenWidth, uint32_t InScreenHeight) override
	{
		float MouseX = float(InX) - InScreenWidth * 0.5f;
		float MouseY = float(-InY) + InScreenHeight * 0.5f;
		CMouseManager::GetInst().SetMousePosition({ MouseX, MouseY });
	}
};

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
	: Core::CApplication(InScreenWidth, InScreenHeight)
	, SpriteRenderer(std::make_unique<Graphics::DX::CDXInfra>(Window.GetWindowHandle(), Window.GetScreenWidth(), Window.GetScreenHeight())
		, Window.GetScreenWidth(), Window.GetScreenHeight())
	, InputActionManager(InputManager)
	, MouseInteractionManager()
	, World()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr)) assert(0);

	Window.RegistMouseMoveEvent(std::make_unique<CSetMousePositionToMouseManager>());

	World.Start();
}

CGame::~CGame()
{
	CoUninitialize();
}

bool CGame::Process()
{
	CCoreSystem::GetInst().ArrangeObjects();

	InputManager.Update();
	CMouseManager::GetInst().SetStateByInputManager(InputManager);

	World.Arrange();
	World.Ready();

	World.PerformInputAction(InputActionManager);
	World.DetectMouseInteraction(MouseInteractionManager);

	World.Update();

	World.CaptureSnapShot(SpriteRenderer);
	World.RenderWorld(SpriteRenderer);

	MouseInteractionManager.ClearState();
	return true;
}

void CGame::ShutDown()
{
	CCoreSystem::GetInst().ShutDown();
}