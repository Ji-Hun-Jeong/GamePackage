#pragma once
#include "01.Base/Actor/Scene.h"
#include "02.Contents/Actor/Edit/ActorGenerator.h"
#include "02.Contents/Actor/Tile/TileManager.h"
#include "04.Renderer/ImGuiManager.h"

enum class EEditMode
{
	Free,
	Tile,
};
class CMapEditorScene : public CScene
{
	GENERATE_OBJECT(CMapEditorScene)
public:
	CMapEditorScene()
	{
		InteractionComponent = AddComponent<CInteractionComponent>();
		InteractionComponent->SetRectScale(1.0f, 1.0f); // 00이면 계속 로그떠서
	}
	~CMapEditorScene() = default;

public:
	void BeginPlay() override;
	void Update(float InDeltaTime) override;
	void CaptureSnapShot() override;

private:
	// 원래는 모드를 두는게 좋을것같은데 그냥 일단 씬에 때려박자 나중에 ㄱㄱ
	EEditMode EditMode = EEditMode::Free;

	CActorGenerator* ActorGenerator = nullptr;
	bool bOpenWindowDialog = false;

	CTileManager* TileManager = nullptr;
	size_t TileWidth = 90;
	size_t TileHeight = 60;
	size_t TileMapRow = 30;
	size_t TileMapCol = 30;
	ETilePutMode TilePutMode = ETilePutMode::Center;
	bool bLayTiles = false;

};

