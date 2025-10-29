#pragma once
#include "01.Base/Actor/Scene.h"

class CTestScene : public CScene
{
	GENERATE_OBJECT(CTestScene)
public:
	CTestScene() = default;
	~CTestScene() = default;

public:
	void Initalize() override;
private:

};

