#pragma once
#include "Window.h"
#include "InputManager.h"

namespace Core
{
	class CApplication
	{
	public:
		CApplication(UINT InScreenWidth, UINT InScreenHeight);
		CApplication(const CApplication&) = delete;
		CApplication(CApplication&&) = delete;
		virtual ~CApplication() = default;

	public:
		virtual bool Process() = 0;
		virtual void ShutDown() = 0;

	protected:
		CWindow Window;

	};
}