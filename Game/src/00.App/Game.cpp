#include "pch.h"
#include "Game.h"
#include <Renderer/Platform/DX/DXInfra.h>

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
	: Core::CApplication(InScreenWidth, InScreenHeight)
	, SpriteRenderer(std::make_unique<Graphics::DX::CDXInfra>(Window.GetWindowHandle(), Window.GetScreenWidth(), Window.GetScreenHeight())
		, Window.GetScreenWidth(), Window.GetScreenHeight())
	, InputManager(Window)
	, InputActionManager(InputManager)
	, World()
{
	SpriteRenderer.InitalizeFromWorld(World);
	InputActionManager.InitalizeFromWorld(World);
	World.Start();
}

CGame::~CGame()
{
}

bool CGame::Process()
{
	InputActionManager.PerformAction();
	World.Update();
	World.Arrange();
	SpriteRenderer.Render();

	return true;
}

void CGame::ShutDown()
{
	
}