#include "pch.h"
#include "World.h"
#include "Core/public/Window.h"
#include "02.Contents/Scene/TestScene/TestScene.h"

CWorld::CWorld(Core::CWindow& InWindow)
	: Window(InWindow)
	, InputManager(Window)
	, CurrentScene(nullptr)
{
	AddScene(new TestScene(*this));
	SetCurrentScene("Test");
}

CWorld::~CWorld()
{
}

