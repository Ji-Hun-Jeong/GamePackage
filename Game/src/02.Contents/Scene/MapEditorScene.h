#pragma once
#include "01.Base/Actor/Scene.h"

class CMapEditorScene : public CScene
{
	GENERATE_OBJECT(CMapEditorScene)
public:
	CMapEditorScene() = default;
	~CMapEditorScene() = default;

public:
	void BeginPlay() override;

private:

};

