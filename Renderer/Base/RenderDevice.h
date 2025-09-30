#pragma once
#include <iostream>

namespace Graphics
{
	class IRenderDevice
	{
	public:
		IRenderDevice(HWND _WindowHandle)
			: WindowHandle(_WindowHandle)
		{
			RECT r = { 0 };
			::GetWindowRect(WindowHandle, &r);
			ScreenWidth = r.right - r.left;
			ScreenHeight = r.bottom - r.top;
		}
		virtual ~IRenderDevice() = 0 {}

	public:
		void SetScreenSize(UINT InScreenWidth, UINT InScreenHeight)
		{
			ScreenWidth = InScreenWidth;
			ScreenHeight = InScreenHeight;
		}

	protected:
		HWND WindowHandle;
		UINT ScreenWidth;
		UINT ScreenHeight;

	};
}