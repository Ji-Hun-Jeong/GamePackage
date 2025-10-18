#pragma once
#include "01.Base/Object/Scene.h"

class CScene2 : public CScene
{
	GENERATE_OBJECT()
	DONTCOPY(CScene2)
public:
	CScene2() = default;
public:
	void Initalize() override;
};

