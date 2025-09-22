#pragma once
#include <Core/public/Application.h>

class CGame : public Core::CApplication
{
	DONTCOPY(CGame)
public:
	CGame(UINT InScreenWidth, UINT InScreenHeight);
	~CGame();
	
public:
	bool Process() override;
	void ShutDown() override;

private:
	
};

