#include "pch.h"
#include "Game.h"
#include <Renderer/Platform/DX/DXInfra.h>

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
	: Core::CApplication(InScreenWidth, InScreenHeight)
	, Renderer(std::make_unique<Graphics::DX::CDXInfra>(Window.GetWindowHandle()))
	, World()
{
	Renderer.InitalizeFromWorld(World);
	World.Start();
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