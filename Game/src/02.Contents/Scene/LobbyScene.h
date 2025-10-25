#pragma once
#include "01.Base/Object/Scene.h"

class CLobbyScene : public CScene
{
	GENERATE_OBJECT(CLobbyScene)
	DONTCOPY(CLobbyScene)
public:
	CLobbyScene() = default;
	~CLobbyScene() = default;

public:
	void Initalize() override;

private:

};

