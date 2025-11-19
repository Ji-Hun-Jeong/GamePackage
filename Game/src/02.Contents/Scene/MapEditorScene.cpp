#include "pch.h"
#include "MapEditorScene.h"

#include "GameCore.h"
#include "00.App/WindowManager.h"

CMapEditorScene::CMapEditorScene()
{
	InteractionComponent = AddComponent<CInteractionComponent>();
	InteractionComponent->SetRectScale(5000.0f, 5000.0f); 
}

void CMapEditorScene::BeginPlay()
{
	CScene::BeginPlay();
	GetFader()->FadeIn(0.0f);

	ActorGenerator = GetWorld()->SpawnActor<CActorGenerator>(this);
	TileManager = GetWorld()->SpawnActor<CTileManager>(this);
	TileSnapUI = GetWorld()->SpawnActor<CTileSnapUI>(this);
}

void CMapEditorScene::Update(float InDeltaTime)
{
	CScene::Update(InDeltaTime);
	
	if (bOpenWindowDialog)
	{
		ActorGenerator->SetGeneratedActorImagePathByWindowManager(CWindowManager::GetInst());
		bOpenWindowDialog = false;
	}

	if (bLayTiles)
	{
		TileManager->LayTiles(*ActorGenerator, TileWidth, TileHeight, TileMapRow, TileMapCol);
		bLayTiles = false;
	}

	if (InteractionComponent->IsMouseFocus() == false)
		return;

	const int32_t MouseX = InteractionComponent->GetMouseInteracter()->GetCurrentMouseX();
	const int32_t MouseY = InteractionComponent->GetMouseInteracter()->GetCurrentMouseY();
	Vector2 MouseWorld2DPosition = Vector2(float(MouseX), float(MouseY));

	switch (EditMode)
	{
	case EEditMode::Free:
		if (LClicked())
			ActorGenerator->GenerateStaticActor(MouseWorld2DPosition);
		if (RClicked())
			ActorGenerator->ErasePrevGeneratedActor();
		break;
	case EEditMode::Tile:
	{
		if (LHold())
			TileManager->PutOnActorToProximateTile(*ActorGenerator, MouseWorld2DPosition);
		else if (RHold())
			TileManager->PutOffActorToProximateTile(*ActorGenerator, MouseWorld2DPosition);
	}
	break;
	case EEditMode::Attach:
	{
		// 왼클릭으로 타일을 쭉 선택하고 선택한 타일들을 위치를 바꿀 수 있는 ui띄우고 그 ui를 누른쪽으로 Move
		if (LClicked())
		{
			CTile* ProximateTile = TileManager->GetProximateTile(MouseWorld2DPosition);
			if (ProximateTile == nullptr)
				break;
			TileManager->SnapOnTileActor(*ProximateTile, MouseWorld2DPosition);
		}
		if (LHold() && GetKey(EKeyType::Q, EButtonState::Hold))
		{
			CTile* ProximateTile = TileManager->GetProximateTile(MouseWorld2DPosition);
			if (ProximateTile == nullptr)
				break;
			TileManager->ChooseTile(*ProximateTile);
		}
		if (RClicked())
			TileManager->ClearChooseTiles();
	}
	break;
	default:
		break;
	}

}

void CMapEditorScene::CaptureSnapShot()
{
	CScene::CaptureSnapShot();

	const char* ModeNames[] = { "Free", "Tile", "Attach" };
	int CurrentIndex = static_cast<int>(EditMode);
	if (ImGui::Combo("Game Mode", &CurrentIndex, ModeNames, static_cast<int>(EEditMode::End)))
		EditMode = static_cast<EEditMode>(CurrentIndex);

	if (ImGui::Button("LayTiles"))
		bLayTiles = true;
	ImGui::InputScalar("TileWidth", ImGuiDataType_U64, &TileWidth);
	ImGui::InputScalar("TileHeight", ImGuiDataType_U64, &TileHeight);
	ImGui::InputScalar("TileMapRow", ImGuiDataType_U64, &TileMapRow);
	ImGui::InputScalar("TileMapCol", ImGuiDataType_U64, &TileMapCol);
	
	if (ImGui::Button("OpenWindowDialog"))
		bOpenWindowDialog = true;
	ImGui::BeginChild("TileList", ImVec2(0, 0), true);
	const std::wstring& GeneratedActorImagePath = ActorGenerator->GetGeneratedActorImagePath();
	for (auto& Pair : ActorGenerator->GetLoadedImagePaths())
	{
		const std::string& ImageName = Pair.first;
		const std::wstring& ImagePath = Pair.second;

		bool bSelected = (GeneratedActorImagePath == ImagePath);

		if (ImGui::Selectable(ImageName.c_str(), bSelected))
		{
			ActorGenerator->SetGeneratedActorImagePath(ImagePath);
			ImGui::SetItemDefaultFocus();
		}
	}
	ImGui::EndChild();
}