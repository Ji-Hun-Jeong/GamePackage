#pragma once
#include "01.Base/Actor/Scene.h"
#include "02.Contents/Actor/Tile/TileMap.h"
#include "02.Contents/Actor/Tile/TileSnapUI.h"
#include "02.Contents/Actor/Tile/TileInteractionHandler.h"
#include "02.Contents/Actor/Collision/GroundManager.h"

#include "State/LadderEditState.h"

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

	void FreeMode();
	void TileMode();
	void LadderMode();
	void ColliderMode();

	void ChangeMode(EEditMode InEditMode);

private:
	// 원래는 모드를 두는게 좋을것같은데 그냥 일단 씬에 때려박자 나중에 ㄱㄱ
	EEditMode EditMode = EEditMode::Ladder;
	IEditState* CurrentEditState = nullptr;
	CUI* MainPanel = nullptr;


	CImageImporter ImageImporter;
	CActorGenerator ActorGenerator;
	bool bOpenWindowDialog = false;

	CTileMap* TileMap = nullptr;
	size_t TileWidth = 90;
	size_t TileHeight = 60;
	size_t TileMapRow = 30;
	size_t TileMapCol = 30;
	bool bLayTiles = false;

	CTileSnapUI* TileSnapUI = nullptr;

	CLadderEditState* LadderEditState = nullptr;

	std::unique_ptr<CTileInteractionHandler> TileInteractionHandler;

	CGroundManager* GroundManager = nullptr;
	bool bPlaceGround = false;

};

