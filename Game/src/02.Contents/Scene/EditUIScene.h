#pragma once
#include "01.Base/Object/Scene.h"


class CEditUIScene : public CScene
{
	GENERATE_OBJECT()
	DONTCOPY(CEditUIScene)
public:
	CEditUIScene() = default;
	~CEditUIScene() = default;

public:
	void Initalize() override;
	void BeginPlay() override
	{
	}

};

