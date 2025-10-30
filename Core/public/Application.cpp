#include "pch.h"
#include "Application.h"

namespace Core
{
	CApplication::CApplication(UINT InScreenWidth, UINT InScreenHeight)
		: Window(InScreenWidth, InScreenHeight)
		, InputManager()
	{

	}
}