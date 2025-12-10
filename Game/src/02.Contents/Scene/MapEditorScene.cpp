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
	GetFader()->FadeIn(1.0f);

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

	GroundManager = GetWorld()->SpawnActor<CGroundManager>(this);

}

void CMapEditorScene::Update(float InDeltaTime)
{
	CScene::Update(InDeltaTime);

	switch (EditMode)
	{
	case EEditMode::Free:
		FreeMode();
		break;
	case EEditMode::Tile:
		TileMode();
		break;
	case EEditMode::Ladder:
		LadderMode();
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
		ActorGenerator.GenerateStaticActor(ImageImporter, MouseWorld2DPosition);
	//else if (RClicked())
	//{
	//	// Test
	//	ActorGenerator->ErasePrevGeneratedActor();
	//}
}

void CMapEditorScene::TileMode()
{
	if (bOpenWindowDialog)
	{
		ImageImporter.AddImagePathByWindowManager(CWindowManager::GetInst());
		bOpenWindowDialog = false;
	}

	if (bLayTiles)
	{
		TileMap->LayTiles(TileWidth, TileHeight, TileMapRow, TileMapCol);
		bLayTiles = false;
	}

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
		if (ImageImporter.IsExistCurrentImagePath() == false)
			return;

		const std::wstring& GeneratedActorImagePath = ImageImporter.GetCurrentImagePath();

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
				TilePutOnActor->Destroy();
				TileMap->CutActorOnTile(ProximateTileKey);
				FinalPutOnActor = TileMap->GetPutOnActor(ProximateTileKey);
			}
		}

		if (FinalPutOnActor == nullptr)
		{
			// Todo: Generate nullptr
			Vector2 ProximateTileWorldPosition = ProximateTile->GetTransform()->GetFinalPosition2D();
			FinalPutOnActor = ActorGenerator.GenerateStaticActor(ImageImporter, ProximateTileWorldPosition);
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

		if (TilePutOnActor)
			TilePutOnActor->Destroy();
		TileMap->CutActorOnTile(ProximateTileKey);
		TileInteractionHandler->RemoveHandledTile(ProximateTileKey);
	}
}

void CMapEditorScene::LadderMode()
{
	if (bSetLadderHead)
	{
		std::wstring LadderHeadImagePath;
		if (CWindowManager::GetInst().TryGetFilePathByDialog(&LadderHeadImagePath))
			LadderEditor.SetHeadImagePath(LadderHeadImagePath);
		bSetLadderHead = false;
	}
	if (bSetLadderBody)
	{
		std::wstring LadderBodyImagePath;
		if (CWindowManager::GetInst().TryGetFilePathByDialog(&LadderBodyImagePath))
			LadderEditor.AddBodyImagePath(LadderBodyImagePath);
		bSetLadderBody = false;
	}
	if (bSetLadderFoot)
	{
		std::wstring LadderFootImagePath;
		if(CWindowManager::GetInst().TryGetFilePathByDialog(&LadderFootImagePath))
			LadderEditor.SetFootImagePath(LadderFootImagePath);
		bSetLadderFoot = false;
	}
	if (InteractionComponent->IsMouseFocus() == false)
		return;
	const int32_t MouseX = InteractionComponent->GetMouseInteracter()->GetCurrentMouseX();
	const int32_t MouseY = InteractionComponent->GetMouseInteracter()->GetCurrentMouseY();
	Vector2 MouseWorld2DPosition = Vector2(float(MouseX), float(MouseY));

	if (LClicked())
	{
		CLadder* ProximateLadder = LadderEditor.GetLadder(MouseWorld2DPosition);
		if (ProximateLadder)
			LadderEditor.SetFocusLadder(ProximateLadder);
		else
		{
			if (LadderEditor.IsEditReady() == false)
				return;
			CLadder* Ladder = GetWorld()->SpawnActor<CLadder>(this);
			LadderEditor.SetLadder(*Ladder, Vector3(MouseWorld2DPosition.x, MouseWorld2DPosition.y, 1.0f));
		}
	}
	else if (RClicked())
	{
		CLadder* FocusLadder = LadderEditor.GetFocusLadder();
		if (FocusLadder)
			FocusLadder->StretchToDown();
	}

	CLadder* FocusLadder = LadderEditor.GetFocusLadder();
	if (FocusLadder)
	{
		static CStaticActor* FocusActorRenderer = GetWorld()->SpawnActor<CStaticActor>(this);
		FocusActorRenderer->SetLineActor();
		FocusActorRenderer->GetTransform()->SetPosition(FocusLadder->GetTransform()->GetFinalPosition());
		FocusActorRenderer->GetTransform()->SetScale(FocusLadder->GetTransform()->GetScale());
		FocusActorRenderer->GetSpriteRenderComponent()->SetColor(Vector3(1.0f, 0.0f, 0.0f), 1.0f);
	}
	
}

void CMapEditorScene::ColliderMode()
{
	if (bPlaceGround)
	{
		TileInteractionHandler->SetGroundByHandledTiles(*GroundManager);
		bPlaceGround = false;
	}

	if (InteractionComponent->IsMouseFocus() == false)
		return;
	const int32_t MouseX = InteractionComponent->GetMouseInteracter()->GetCurrentMouseX();
	const int32_t MouseY = InteractionComponent->GetMouseInteracter()->GetCurrentMouseY();
	Vector2 MouseWorld2DPosition = Vector2(float(MouseX), float(MouseY));

	if (LHold())
	{
		TileKey ProximateTileKey = TileMap->GetProximateTile(MouseWorld2DPosition);
		if (TileMap->IsValidateKey(ProximateTileKey))
			TileInteractionHandler->AddHandledTile(ProximateTileKey);
	}
	else if (RHold())
	{
		TileKey ProximateTileKey = TileMap->GetProximateTile(MouseWorld2DPosition);
		if (TileMap->IsValidateKey(ProximateTileKey))
			TileInteractionHandler->RemoveHandledTile(ProximateTileKey);
		GroundManager->RemoveProximateCollider(MouseWorld2DPosition);
	}
}

void CMapEditorScene::ChangeMode(EEditMode InEditMode)
{
	TileInteractionHandler->ClearHandledTiles();
	TileSnapUI->DisappearUI();
	switch (InEditMode)
	{
	case EEditMode::Free:
		break;
	case EEditMode::Tile:
		break;
	case EEditMode::Ladder:
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

	const char* ModeNames[] = { "Free", "Tile", "Ladder", "Collider" };
	int CurrentIndex = static_cast<int>(EditMode);
	if (ImGui::Combo("Game Mode", &CurrentIndex, ModeNames, static_cast<int>(EEditMode::End)))
	{
		if (EditMode != static_cast<EEditMode>(CurrentIndex))
			ChangeMode(static_cast<EEditMode>(CurrentIndex));
	}

	switch (EditMode)
	{
	case EEditMode::Free:
		break;
	case EEditMode::Tile:
	{
		if (ImGui::Button("LayTiles"))
			bLayTiles = true;
		ImGui::InputScalar("TileWidth", ImGuiDataType_U64, &TileWidth);
		ImGui::InputScalar("TileHeight", ImGuiDataType_U64, &TileHeight);
		ImGui::InputScalar("TileMapRow", ImGuiDataType_U64, &TileMapRow);
		ImGui::InputScalar("TileMapCol", ImGuiDataType_U64, &TileMapCol);

		if (ImGui::Button("OpenWindowDialog"))
			bOpenWindowDialog = true;

		ImGui::BeginChild("TileList", ImVec2(0, 0), true);

		const std::wstring& GeneratedActorImagePath = ImageImporter.GetCurrentImagePath();
		for (auto& Pair : ImageImporter.GetLoadedImagePaths())
		{
			const std::string& ImageName = Pair.first;
			const std::wstring& ImagePath = Pair.second;

			bool bSelected = (GeneratedActorImagePath == ImagePath);

			if (ImGui::Selectable(ImageName.c_str(), bSelected))
			{
				ImageImporter.SetCurrentImagePath(ImageName);
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndChild();
	}
	break;
	case EEditMode::Ladder:
	{
		if (ImGui::Button("SetLadderHead"))
			bSetLadderHead = true;
		if (ImGui::Button("SetLadderBody"))
			bSetLadderBody = true;
		if (ImGui::Button("SetLadderFoot"))
			bSetLadderFoot = true;
	}
	break;
	case EEditMode::Collider:
	{
		if (ImGui::Button("PlaceGround"))
			bPlaceGround = true;
	}
	break;
	case EEditMode::End:
		break;
	default:
		break;
	}
}