#include "pch.h"
#include "Core.h"

namespace Core
{
	bool CCore::Run()
	{
		static MSG msg;

		while (true)
		{
			// 메시지 큐에서 메시지 확인
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				if (Application.Process() == false)
					break;
			}
		}
		Application.ShutDown();
		return false;
	}
}