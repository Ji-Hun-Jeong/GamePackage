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
	TileMap = GetWorld()->SpawnActor<CTileMap>(this);
	TileSnapUI = GetWorld()->SpawnActor<CTileSnapUI>(this);

	TileInteractionHandler = std::make_unique<CTileInteractionHandler>(*TileMap);

	TileSnapUI->SetUIEvent(ETilePositionType::Center, [this]()->void {if (LClicked())TileInteractionHandler->MoveHandledTiles(ETilePositionType::Center); });
	TileSnapUI->SetUIEvent(ETilePositionType::Left, [this]()->void {if (LClicked())TileInteractionHandler->MoveHandledTiles(ETilePositionType::Left); });
	TileSnapUI->SetUIEvent(ETilePositionType::Right, [this]()->void {if (LClicked())TileInteractionHandler->MoveHandledTiles(ETilePositionType::Right); });
	TileSnapUI->SetUIEvent(ETilePositionType::Top, [this]()->void {if (LClicked())TileInteractionHandler->MoveHandledTiles(ETilePositionType::Top); });
	TileSnapUI->SetUIEvent(ETilePositionType::Bottom, [this]()->void {if (LClicked())TileInteractionHandler->MoveHandledTiles(ETilePositionType::Bottom); });
	TileSnapUI->SetUIEvent(ETilePositionType::LeftTop, [this]()->void {if (LClicked())TileInteractionHandler->MoveHandledTiles(ETilePositionType::LeftTop); });
	TileSnapUI->SetUIEvent(ETilePositionType::LeftBottom, [this]()->void {if (LClicked())TileInteractionHandler->MoveHandledTiles(ETilePositionType::LeftBottom); });
	TileSnapUI->SetUIEvent(ETilePositionType::RightTop, [this]()->void {if (LClicked())TileInteractionHandler->MoveHandledTiles(ETilePositionType::RightTop); });
	TileSnapUI->SetUIEvent(ETilePositionType::RightBottom, [this]()->void {if (LClicked())TileInteractionHandler->MoveHandledTiles(ETilePositionType::RightBottom); });

	/*CStaticActor* Actor = GetWorld()->SpawnActor<CStaticActor>(this);
	Actor->GetTransform()->SetScale(Vector3(90.0f, 60.0f, 0.0f));
	Actor->GetSpriteRenderComponent()->SetPSO(EPSOType::Edge);
	Actor->GetSpriteRenderComponent()->SetColor(Vector3(0.0f, 0.0f, 0.0f), 1.0f);
	Actor->GetSpriteRenderComponent()->SetEdge(Vector3(1.0f, 0.0f, 0.0f), 2, Actor->GetTransform()->GetScale().x, Actor->GetTransform()->GetScale().y);*/
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
		ActorGenerator->ClearActor();
		TileMap->LayTiles(TileWidth, TileHeight, TileMapRow, TileMapCol);
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
	case EEditMode::Collider:
		ColliderMode();
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
	{
		if (GetKey(EKeyType::Ctrl, EButtonState::Hold) == false)
			TileInteractionHandler->ClearHandledTiles();
	}
	else if (LHold())
	{
		TileKey ProximateTileKey = TileMap->GetProximateTile(MouseWorld2DPosition);
		if (TileMap->IsValidateKey(ProximateTileKey) == false)
		{
			TileInteractionHandler->ClearHandledTiles();
			TileSnapUI->DisappearUI();
			return;
		}
		const std::wstring& GeneratedActorImagePath = ActorGenerator->GetGeneratedActorImagePath();
		if (GeneratedActorImagePath.empty())
			return;

		CTile* ProximateTile = TileMap->GetTile(ProximateTileKey);
		CStaticActor* TilePutOnActor = TileMap->GetPutOnActor(ProximateTileKey);

		CStaticActor* FinalPutOnActor = nullptr;
		if (TilePutOnActor)
		{
			const std::wstring& TilePutOnActorImagePath = TilePutOnActor->GetSpriteRenderComponent()->GetImagePath();
			if (TilePutOnActorImagePath == GeneratedActorImagePath)
				FinalPutOnActor = TilePutOnActor;
			else
			{
				ActorGenerator->EraseActor(*TilePutOnActor);
				TileMap->CutActorOnTile(ProximateTileKey);
				FinalPutOnActor = TileMap->GetPutOnActor(ProximateTileKey);
			}
		}

		if (FinalPutOnActor == nullptr)
		{
			// Todo: Generate nullptr
			Vector2 ProximateTileWorldPosition = ProximateTile->GetTransform()->GetFinalPosition2D();
			FinalPutOnActor = ActorGenerator->GenerateStaticActor(ProximateTileWorldPosition);
			TileMap->PutActorOnTile(*FinalPutOnActor, ProximateTileKey);
		}

		TileInteractionHandler->AddHandledTile(ProximateTileKey);
	}
	else if (LReleased())
	{
		bool bAdjustPosition = TileInteractionHandler->AdjustTileSnapUIPosition(*TileSnapUI);
		if (bAdjustPosition)
			TileSnapUI->AppearUI();
	}
	else if (RHold())
	{
		TileInteractionHandler->ClearHandledTiles();
		TileSnapUI->DisappearUI();
		TileKey ProximateTileKey = TileMap->GetProximateTile(MouseWorld2DPosition);
		if (TileMap->IsValidateKey(ProximateTileKey) == false)
			return;

		CTile* ProximateTile = TileMap->GetTile(ProximateTileKey);
		CStaticActor* TilePutOnActor = TileMap->GetPutOnActor(ProximateTileKey);

		ActorGenerator->EraseActor(*TilePutOnActor);
		TileMap->CutActorOnTile(ProximateTileKey);
		TileInteractionHandler->RemoveHandledTile(ProximateTileKey);
	}
}

void CMapEditorScene::ColliderMode()
{
	if (InteractionComponent->IsMouseFocus() == false)
		return;
	const int32_t MouseX = InteractionComponent->GetMouseInteracter()->GetCurrentMouseX();
	const int32_t MouseY = InteractionComponent->GetMouseInteracter()->GetCurrentMouseY();
	Vector2 MouseWorld2DPosition = Vector2(float(MouseX), float(MouseY));
}

void CMapEditorScene::ChangeMode(EEditMode InEditMode)
{
	switch (InEditMode)
	{
	case EEditMode::Free:
		break;
	case EEditMode::Tile:
		break;
	case EEditMode::Collider:
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

	const char* ModeNames[] = { "Free", "Tile", "Collider" };
	int CurrentIndex = static_cast<int>(EditMode);
	if (ImGui::Combo("Game Mode", &CurrentIndex, ModeNames, static_cast<int>(EEditMode::End)))
	{
		if (EditMode != static_cast<EEditMode>(CurrentIndex))
			ChangeMode(static_cast<EEditMode>(CurrentIndex));
	}

	if (EditMode == EEditMode::Tile)
	{
		if (ImGui::Button("LayTiles"))
			bLayTiles = true;
		ImGui::InputScalar("TileWidth", ImGuiDataType_U64, &TileWidth);
		ImGui::InputScalar("TileHeight", ImGuiDataType_U64, &TileHeight);
		ImGui::InputScalar("TileMapRow", ImGuiDataType_U64, &TileMapRow);
		ImGui::InputScalar("TileMapCol", ImGuiDataType_U64, &TileMapCol);
	}

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