#pragma once

extern LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
namespace Core
{
	class IWindowResize
	{
		friend LRESULT CALLBACK ::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void WindowResize(UINT InNewScreenWidth, UINT InNewScreenHeight) = 0;
	};
	class IWindowMove
	{
		friend LRESULT CALLBACK::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void WindowMove(int InX, int InY) = 0;
	};
	class IMouseMove
	{
		friend LRESULT CALLBACK::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void MouseMove(int InX, int InY) = 0;
	};
}