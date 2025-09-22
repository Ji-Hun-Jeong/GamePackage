#pragma once
#include "Application.h"

namespace Core
{
	class CCore final
	{
	public:
		explicit CCore(CApplication& InApplication)
			: Application(InApplication)
		{}
		~CCore()
		{
		}

	public:
		bool Run();

	private:
		CApplication& Application;

	};
}

