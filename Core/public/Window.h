#pragma once
#include "../private/Event/WindowEvent.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace Core
{
	class CWindow final
	{
		friend LRESULT CALLBACK ::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	public:
		CWindow(UINT InScreenWidth, UINT InScreenHeight);
		~CWindow();

	public:
		IWindowResize* RegistWindowResizeEvent(std::unique_ptr<IWindowResize> InWindowResizeEvent)
		{
			IWindowResize* WindowResizeEvent = InWindowResizeEvent.get();
			WindowResizeEvents.push_back(std::move(InWindowResizeEvent));
			return WindowResizeEvent;
		}
		IWindowMove* RegistWindowMoveEvent(std::unique_ptr<IWindowMove> InWindowMoveEvent)
		{
			IWindowMove* WindowMoveEvent = InWindowMoveEvent.get();
			WindowMoveEvents.push_back(std::move(InWindowMoveEvent));
			return WindowMoveEvent;
		}
		IMouseMove* RegistMouseMoveEvent(std::unique_ptr<IMouseMove> InMouseMoveEvent)
		{
			IMouseMove* MouseMoveEvent = InMouseMoveEvent.get();
			MouseMoveEvents.push_back(std::move(InMouseMoveEvent));
			return MouseMoveEvent;
		}

	private:
		HWND WindowHandle;
		UINT ScreenWidth;
		UINT ScreenHeight;

		std::vector<std::unique_ptr<IWindowResize>> WindowResizeEvents;
		std::vector<std::unique_ptr<IWindowMove>> WindowMoveEvents;
		std::vector<std::unique_ptr<IMouseMove>> MouseMoveEvents;

	};

}
