#pragma once
#include "01.Base/Actor/Scene.h"
#include "02.Contents/Actor/Base/ImGUIInteracter.h"

#include "State/FreeEditState.h"
#include "State/LadderEditState.h"
#include "State/TileEditState.h"
#include "State/ColliderEditState.h"

enum class EEditMode
{
	Free,
	Tile,
	Ladder,
	Collider,
	End,
};

class CMapEditorScene : public CScene
{
	GENERATE_OBJECT(CMapEditorScene)
public:
	CMapEditorScene();
	~CMapEditorScene() = default;

public:
	void BeginPlay() override;
	void Update(float InDeltaTime) override;
	void CaptureSnapShot() override;

	void ChangeMode(EEditMode InEditMode);

private:
	// 원래는 모드를 두는게 좋을것같은데 그냥 일단 씬에 때려박자 나중에 ㄱㄱ
	EEditMode EditMode = EEditMode::Tile;
	CEditState* CurrentEditState = nullptr;

	TTileMapEditContext EditContext;

	CFreeEditState* FreeEditState = nullptr;
	CLadderEditState* LadderEditState = nullptr;
	CTileEditState* TileEditState = nullptr;
	CColliderEditState* ColliderEditState = nullptr;

	size_t TileWidth = 90;
	size_t TileHeight = 60;
	size_t TileMapRow = 30;
	size_t TileMapCol = 30;
	bool bLayTiles = false;

	CImGUIInteracter* ImGUIInteracter = nullptr;

};

