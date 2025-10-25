#include "pch.h"
#include "World.h"
#include "02.Contents/Scene/LobbyScene.h"
#include "02.Contents/Scene/EditUIScene.h"

CWorld* g_World = nullptr;

CWorld::CWorld()
	: bFlagDestroyedWorldObject(false)
{
	g_World = this;
}

CWorld::~CWorld()
{

}

void CWorld::Start()
{
	LoadScene<CLobbyScene>();
}
