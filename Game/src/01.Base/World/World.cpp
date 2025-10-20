#include "pch.h"
#include "World.h"
#include "02.Contents/Scene/LobbyScene.h"

CWorld::CWorld()
	: bFlagDestroyedWorldObject(false)
{

}

CWorld::~CWorld()
{

}

void CWorld::Start()
{
	LoadScene<CLobbyScene>();
}
