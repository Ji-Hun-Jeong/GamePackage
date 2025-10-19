#include "pch.h"
#include "Game.h"
#include <Renderer/Platform/DX/DXInfra.h>

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
	: Core::CApplication(InScreenWidth, InScreenHeight)
	, SpriteRenderer(std::make_unique<Graphics::DX::CDXInfra>(Window.GetWindowHandle(), Window.GetScreenWidth(), Window.GetScreenHeight())
		, Window.GetScreenWidth(), Window.GetScreenHeight())
	, InputManager(Window)
	, InputActionManager(InputManager)
	, MouseManager()
	, World()
{
	//SpriteRenderer.InitalizeFromWindow(Window);
	SpriteRenderer.InitalizeFromWorld(World);
	InputActionManager.InitalizeFromWorld(World);
	MouseManager.InitalizeFromWorld(World);

	Binding();

	World.Start();
}

CGame::~CGame()
{
}

bool CGame::Process()
{
	World.Arrange();

	World.Ready();
	World.Update();

	InputActionManager.PerformAction();
	MouseManager.SetNDCMousePosition(InputManager.GetMouseNDCPosition().MouseX, InputManager.GetMouseNDCPosition().MouseY);
	MouseManager.FindCurrentInteracter();

	World.CaptureSnapShot();

	SpriteRenderer.Render();
	return true;
}

void CGame::ShutDown()
{
}