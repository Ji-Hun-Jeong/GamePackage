#include "pch.h"
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#include <Core/public/Core.h>
#include "Game/Game.h"

int main(int InArgc, char** InArgv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	new int;

	CGame Game(1280, 960);
	Core::CCore Core(Game);
	Core.Run();

}