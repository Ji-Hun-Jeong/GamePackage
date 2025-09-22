#include "pch.h"
#include "Game.h"


class CLogWindowResizeEvent : public Core::IWindowResize
{
	void WindowResize(UINT InNewScreenWidth, UINT InNewScreenHeight) override
	{
		std::cout << InNewScreenWidth << ", " << InNewScreenHeight << "\n";
	}
};

class CLogWindowMoveEvent : public Core::IWindowMove
{
	void WindowMove(int InX, int InY) override
	{
		std::cout << InX << ", " << InY << "\n";
	}
};

class CLogMouseMoveEvent : public Core::IMouseMove
{
	void MouseMove(int InX, int InY) override
	{
		std::cout << InX << ", " << InY << "\n";
	}
};

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
    : Core::CApplication(InScreenWidth, InScreenHeight)
{
	Window.RegistWindowResizeEvent(std::make_unique<CLogWindowResizeEvent>());
	Window.RegistWindowMoveEvent(std::make_unique<CLogWindowMoveEvent>());
	Window.RegistMouseMoveEvent(std::make_unique<CLogMouseMoveEvent>());
}

CGame::~CGame()
{
}

bool CGame::Process()
{
    return true;
}

void CGame::ShutDown()
{
}