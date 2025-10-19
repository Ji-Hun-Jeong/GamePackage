#include "pch.h"
#include "Window.h"
#include <windowsx.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// GWLP_USERDATA에서 CWindow 포인터를 가져옴
	Core::CWindow* Window = reinterpret_cast<Core::CWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	
	switch (uMsg)
	{
	case WM_CREATE:
	{
		CREATESTRUCT* Create = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window = reinterpret_cast<Core::CWindow*>(Create->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)Window);
	}
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (Window) // 포인터가 유효한지 확인
		{
			UINT Width = LOWORD(lParam);
			UINT Height = HIWORD(lParam);
			
			Window->ScreenWidth = Width;
			Window->ScreenHeight = Height;
			for (auto& WindowResizeEvent : Window->WindowResizeEvents)
				WindowResizeEvent->WindowResize(Window->ScreenWidth, Window->ScreenHeight);
		}
		return 0;

	case WM_MOVE:
		if (Window)
		{
			// lParam에서 새로운 x, y 좌표를 추출합니다.
			// windowsx.h의 매크로를 사용하는 것이 안전하고 편리합니다.
			int PosX = GET_X_LPARAM(lParam);
			int PosY = GET_Y_LPARAM(lParam);

			for (auto& WindowMoveEvent : Window->WindowMoveEvents)
				WindowMoveEvent->WindowMove(PosX, PosY);
		}
		return 0;

	case WM_MOUSEMOVE:
		if (Window)
		{
			int PosX = GET_X_LPARAM(lParam);
			int PosY = GET_Y_LPARAM(lParam);

			for (auto& MouseMoveEvent : Window->MouseMoveEvents)
				MouseMoveEvent->MouseMove(PosX, PosY, Window->ScreenWidth, Window->ScreenHeight);
		}
		return 0;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
namespace Core
{
	CWindow::CWindow(UINT InScreenWidth, UINT InScreenHeight)
		: WindowHandle(nullptr)
		, ScreenWidth(InScreenWidth)
		, ScreenHeight(InScreenHeight)
	{
		HINSTANCE hInstance = GetModuleHandle(NULL);

		WNDCLASS wc = {};
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = "MyWindowClass";

		if (!RegisterClass(&wc))
		{
			MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONERROR);
			assert(0);
		}

		RECT rect = { 0, 0, static_cast<LONG>(ScreenWidth), static_cast<LONG>(ScreenHeight) };

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		UINT windowWidth = rect.right - rect.left;
		UINT windowHeight = rect.bottom - rect.top;

		WindowHandle = CreateWindowEx(
			0,
			"MyWindowClass",
			"Main",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			windowWidth,   // <-- 보정된 너비 사용
			windowHeight,  // <-- 보정된 높이 사용
			NULL,
			NULL,
			hInstance,
			this
		);

		if (WindowHandle == NULL)
		{
			MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONERROR);
			assert(0);
		}

		::ShowWindow(WindowHandle, SW_SHOW);
	}
	CWindow::~CWindow()
	{
	}
}