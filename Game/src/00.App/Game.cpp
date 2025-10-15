#include "pch.h"
#include "Game.h"
#include <Renderer/Platform/DX/DXInfra.h>

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
	: Core::CApplication(InScreenWidth, InScreenHeight)
	, World()
	, Renderer(std::make_unique<Graphics::DX::CDXInfra>(Window.GetWindowHandle()))
{
	Renderer.InitalizeFromWorld(World);
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