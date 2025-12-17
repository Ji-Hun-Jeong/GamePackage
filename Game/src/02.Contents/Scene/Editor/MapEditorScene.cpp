#include "pch.h"
#include "MapEditorScene.h"

#include "GameCore.h"
#include "00.App/WindowManager.h"

CMapEditorScene::CMapEditorScene()
{

}

void CMapEditorScene::BeginPlay()
{
	CScene::BeginPlay();
	GetFader()->FadeIn(1.0f);

	EditContext.MainPanel = GetWorld()->SpawnActor<CUI>(this);
	EditContext.MainPanel->GetTransform()->SetScale(Vector3(1270.0f, 950.0f, 1.0f));
	EditContext.TileMap = GetWorld()->SpawnActor<CTileMap>(this);
	EditContext.TileHandler = GetWorld()->SpawnActor<CTileHandler>(this);
	EditContext.TileFocus = GetWorld()->SpawnActor<CTileFocus>(this);

	ImGUIInteracter = GetWorld()->SpawnActor<CImGUIInteracter>(GetMainCamera());
	EditContext.MainPanel->AttachChildUI(*ImGUIInteracter);

	LadderEditState = GetWorld()->SpawnActor<CLadderEditState>(this);
	LadderEditState->InitalizeEditState(EditContext);
	TileEditState = GetWorld()->SpawnActor<CTileEditState>(this);
	TileEditState->InitalizeEditState(EditContext);
	ColliderEditState = GetWorld()->SpawnActor<CColliderEditState>(this);
	ColliderEditState->InitalizeEditState(EditContext);

	ChangeMode(EditMode);

	UIManager.AddUI(*EditContext.MainPanel);
}

void CMapEditorScene::Update(float InDeltaTime)
{
	CScene::Update(InDeltaTime);

	if (bLayTiles)
	{
		EditContext.TileMap->LayTiles(TileWidth, TileHeight, TileMapRow, TileMapCol);
		bLayTiles = false;
	}

	const int32_t MouseX = UIManager.GetMouseWorldPosition().x;
	const int32_t MouseY = UIManager.GetMouseWorldPosition().y;
	Vector2 MouseWorld2DPosition = Vector2(float(MouseX), float(MouseY));

	CurrentEditState->SetMouseWorldPosition(MouseWorld2DPosition);
	CurrentEditState->OnEditState();

}

void CMapEditorScene::ChangeMode(EEditMode InEditMode)
{
	if (CurrentEditState)
		CurrentEditState->ExitEditState();
	switch (InEditMode)
	{
	case EEditMode::Free:
		break;
	case EEditMode::Tile:
		CurrentEditState = TileEditState;
		break;
	case EEditMode::Ladder:
		CurrentEditState = LadderEditState;
		break;
	case EEditMode::Collider:
		CurrentEditState = ColliderEditState;
		break;
	default:
		assert(0);
		break;
	}
	EditMode = InEditMode;
	CurrentEditState->EnterEditState();
}

void CMapEditorScene::CaptureSnapShot()
{
	CScene::CaptureSnapShot();

	const char* ModeNames[] = { "Free", "Tile", "Ladder", "Collider" };
	int CurrentIndex = static_cast<int>(EditMode);
	if (ImGui::Combo("Game Mode", &CurrentIndex, ModeNames, static_cast<int>(EEditMode::End)))
	{
		if (EditMode != static_cast<EEditMode>(CurrentIndex))
			ChangeMode(static_cast<EEditMode>(CurrentIndex));
	}
	
	if (ImGui::Button("LayTiles"))
		bLayTiles = true;
	ImGui::InputScalar("TileWidth", ImGuiDataType_U64, &TileWidth);
	ImGui::InputScalar("TileHeight", ImGuiDataType_U64, &TileHeight);
	ImGui::InputScalar("TileMapRow", ImGuiDataType_U64, &TileMapRow);
	ImGui::InputScalar("TileMapCol", ImGuiDataType_U64, &TileMapCol);

	CurrentEditState->ToImGUI();
}