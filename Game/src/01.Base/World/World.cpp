#include "pch.h"
#include "World.h"
#include "02.Contents/Scene/LobbyScene.h"
#include "02.Contents/Scene/EditUIScene.h"

CWorld::CWorld()
	: bFlagDestroyedWorldObject(false)
{

}

CWorld::~CWorld()
{

}

void CWorld::Start()
{
	LoadScene<CEditUIScene>();
}
