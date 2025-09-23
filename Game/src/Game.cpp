#include "pch.h"
#include "Game.h"

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
    : Core::CApplication(InScreenWidth, InScreenHeight)
    , World(Window)
{

}

CGame::~CGame()
{
}

bool CGame::Process()
{
    World.Update();
    return true;
}

void CGame::ShutDown()
{
}