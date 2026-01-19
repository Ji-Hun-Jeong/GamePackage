#pragma once
#include "01.Base/Actor/Scene.h"
#include "02.Contents/Actor/UI/UI.h"
#include "02.Contents/Actor/Player/Player.h"
#include "02.Contents/Actor/Base/BackGround.h"
#include "02.Contents/Actor/Base/MousePointer.h"

class CEditUIScene : public CScene
{
	GENERATE_OBJECT(CEditUIScene)
	DONTCOPY(CEditUIScene)
public:
	CEditUIScene() = default;
	~CEditUIScene() = default;

public:
	void BeginPlay() override;

};

