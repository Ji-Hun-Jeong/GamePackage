#include "pch.h"
#include "Game.h"
#include <Renderer/Platform/DX/DXInfra.h>

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
	: Core::CApplication(InScreenWidth, InScreenHeight)
	, SpriteRenderer(std::make_unique<Graphics::DX::CDXInfra>(Window.GetWindowHandle(), Window.GetScreenWidth(), Window.GetScreenHeight())
		, Window.GetScreenWidth(), Window.GetScreenHeight())
	, InputActionManager(InputManager)
	, MouseManager()
	, World()
{
	//SpriteRenderer.InitalizeFromWindow(Window);
	SpriteRenderer.InitalizeFromWorld(World);
	InputActionManager.InitalizeFromWorld(World);
	MouseManager.InitalizeFromWorld(World);
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr)) assert(0);

	Binding();

	World.Start();
}

CGame::~CGame()
{
	CoUninitialize();
}

bool CGame::Process()
{
	CCoreSystem::GetInst().ArrangeObjects();

	World.Arrange();
	World.Ready();

	//InputActionManager.PerformAction();
	//MouseManager.FindCurrentInteracter();

	World.Update();

	World.CaptureSnapShot();

	//SpriteRenderer.Render();
	//MouseManager.AddNextInteracter();
	
	return true;
}

void CGame::ShutDown()
{
}