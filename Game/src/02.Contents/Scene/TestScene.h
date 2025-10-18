#pragma once
#include "01.Base/Object/Scene.h"

class CTestScene : public CScene
{
	GENERATE_OBJECT()
	DONTCOPY(CTestScene)
public:
	CTestScene() = default;
	~CTestScene() = default;

public:
	void Initalize() override;

private:

};

