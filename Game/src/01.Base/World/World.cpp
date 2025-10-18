#include "pch.h"
#include "World.h"
#include "02.Contents/Scene/TestScene.h"
#include "02.Contents/Scene/Scene2.h"

CWorld::CWorld()
	: bFlagDestroyedWorldObject(false)
{

}

CWorld::~CWorld()
{

}

void CWorld::Start()
{
	LoadScene<CTestScene>();
}
