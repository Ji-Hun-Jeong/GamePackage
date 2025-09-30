#include "pch.h"
#include "Game.h"


CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
    : Core::CApplication(InScreenWidth, InScreenHeight)
    , World()
{
}

CGame::~CGame()
{
}

bool CGame::Process()
{
    World.Update();
    World.Render();
    return true;
}

void CGame::ShutDown()
{
}