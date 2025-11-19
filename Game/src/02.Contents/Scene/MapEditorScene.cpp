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

	TileSnapUI->SetUIEvent(ETilePositionType::Center, [this]()->void {if (LClicked())TileInteractionHandler.MoveHandledTiles(ETilePositionType::Center); });
	TileSnapUI->SetUIEvent(ETilePositionType::Left, [this]()->void {if (LClicked())TileInteractionHandler.MoveHandledTiles(ETilePositionType::Left); });
	TileSnapUI->SetUIEvent(ETilePositionType::Right, [this]()->void {if (LClicked())TileInteractionHandler.MoveHandledTiles(ETilePositionType::Right); });
	TileSnapUI->SetUIEvent(ETilePositionType::Top, [this]()->void {if (LClicked())TileInteractionHandler.MoveHandledTiles(ETilePositionType::Top); });
	TileSnapUI->SetUIEvent(ETilePositionType::Bottom, [this]()->void {if (LClicked())TileInteractionHandler.MoveHandledTiles(ETilePositionType::Bottom); });
	TileSnapUI->SetUIEvent(ETilePositionType::LeftTop, [this]()->void {if (LClicked())TileInteractionHandler.MoveHandledTiles(ETilePositionType::LeftTop); });
	TileSnapUI->SetUIEvent(ETilePositionType::LeftBottom, [this]()->void {if (LClicked())TileInteractionHandler.MoveHandledTiles(ETilePositionType::LeftBottom); });
	TileSnapUI->SetUIEvent(ETilePositionType::RightTop, [this]()->void {if (LClicked())TileInteractionHandler.MoveHandledTiles(ETilePositionType::RightTop); });
	TileSnapUI->SetUIEvent(ETilePositionType::RightBottom, [this]()->void {if (LClicked())TileInteractionHandler.MoveHandledTiles(ETilePositionType::RightBottom); });

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

	switch (EditMode)
	{
	case EEditMode::Free:
		FreeMode();
		break;
	case EEditMode::Tile:
		TileMode();
		break;
	default:
		break;
	}
}

void CMapEditorScene::FreeMode()
{
	if (InteractionComponent->IsMouseFocus() == false)
		return;

	const int32_t MouseX = InteractionComponent->GetMouseInteracter()->GetCurrentMouseX();
	const int32_t MouseY = InteractionComponent->GetMouseInteracter()->GetCurrentMouseY();
	Vector2 MouseWorld2DPosition = Vector2(float(MouseX), float(MouseY));

	if (LClicked())
		ActorGenerator->GenerateStaticActor(MouseWorld2DPosition);
	else if (RClicked())
		ActorGenerator->ErasePrevGeneratedActor();
}

void CMapEditorScene::TileMode()
{
	if (InteractionComponent->IsMouseFocus() == false)
		return;

	const int32_t MouseX = InteractionComponent->GetMouseInteracter()->GetCurrentMouseX();
	const int32_t MouseY = InteractionComponent->GetMouseInteracter()->GetCurrentMouseY();
	Vector2 MouseWorld2DPosition = Vector2(float(MouseX), float(MouseY));

	if (LClicked())
		TileInteractionHandler.ClearHandledTiles();
	else if (LHold())
	{
		bool bPut = TileInteractionHandler.PutActorOnProximateTile(*ActorGenerator, *TileManager, MouseWorld2DPosition);
		if (bPut == false)
		{
			TileInteractionHandler.ClearHandledTiles();
			TileSnapUI->DisappearUI();
		}
	}
	else if (LReleased())
	{
		bool bAdjustPosition = TileInteractionHandler.AdjustTileSnapUIPosition(*TileSnapUI);
		if (bAdjustPosition)
			TileSnapUI->AppearUI();
	}
	else if (RHold())
	{
		TileInteractionHandler.ClearHandledTiles();
		TileSnapUI->DisappearUI();
		TileInteractionHandler.CutActorOnProximateTile(*ActorGenerator, *TileManager, MouseWorld2DPosition);
	}
}

void CMapEditorScene::ChangeMode(EEditMode InEditMode)
{
	switch (InEditMode)
	{
	case EEditMode::Free:
		break;
	case EEditMode::Tile:
		break;
	default:
		assert(0);
		break;
	}
	EditMode = InEditMode;
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