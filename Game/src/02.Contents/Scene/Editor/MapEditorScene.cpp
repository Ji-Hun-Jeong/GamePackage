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

	MainPanel = GetWorld()->SpawnActor<CUI>(this);
	MainPanel->GetTransform()->SetScale(Vector3(1270.0f, 950.0f, 1.0f));

	LadderEditState = GetWorld()->SpawnActor<CLadderEditState>(this);
	/*TileMap = GetWorld()->SpawnActor<CTileMap>(this);
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

	GroundManager = GetWorld()->SpawnActor<CGroundManager>(this);*/

	ChangeMode(EditMode);
}

void CMapEditorScene::Update(float InDeltaTime)
{
	CScene::Update(InDeltaTime);

	const int32_t MouseX = UIManager.GetMouseWorldPosition().x;
	const int32_t MouseY = UIManager.GetMouseWorldPosition().y;
	Vector2 MouseWorld2DPosition = Vector2(float(MouseX), float(MouseY));
	CurrentEditState->SetMouseWorldPosition(MouseWorld2DPosition);
	CurrentEditState->OnEditState(*MainPanel);

	UIManager.PushUI(*MainPanel);
}

void CMapEditorScene::FreeMode()
{
	/*if (LClicked())
		ActorGenerator.GenerateStaticActor(ImageImporter, MouseWorld2DPosition);*/
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


	const int32_t MouseX = UIManager.GetMouseWorldPosition().x;
	const int32_t MouseY = UIManager.GetMouseWorldPosition().y;
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
			TileSnapUI->Activate(false);
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
			TileSnapUI->Activate(true);
	}
	else if (RHold())
	{
		TileInteractionHandler->ClearHandledTiles();
		TileSnapUI->Activate(false);
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


	/*CLadderForm* FocusLadder = LadderEditor->GetFocusLadder();
	if (FocusLadder)
	{
		static CUI* FocusActorMarker = GetWorld()->SpawnActor<CUI>(this);
		FocusActorMarker->SetLineActor();
		FocusActorMarker->GetTransform()->SetPosition(FocusLadder->GetTransform()->GetFinalPosition());
		FocusActorMarker->GetTransform()->SetScale(FocusLadder->GetTransform()->GetScale());
		FocusActorMarker->GetSpriteRenderComponent()->SetColor(Vector3(1.0f, 0.0f, 0.0f), 1.0f);

		static CUI* StretchUpUI = GetWorld()->SpawnActor<CUI>(this);
		StretchUpUI->SetRectUI(1);
		StretchUpUI->GetTransform()->SetScale(Vector3(20.0f, 20.0f, 0.0f));
		Vector3 UIPosition = CTransformUtils::GetTopPosition(*FocusActorMarker, *StretchUpUI);
		StretchUpUI->GetTransform()->SetPosition(UIPosition);
		StretchUpUI->SetMouseFocusEvent([this]()->void
			{
				if (LClicked())
				{
					CLadderForm* FocusLadder = LadderEditor->GetFocusLadder();
					if (FocusLadder)
						FocusLadder->StretchToUp();
				}
			});
		static CUI* StretchDownUI = GetWorld()->SpawnActor<CUI>(this);
		StretchDownUI->SetRectUI(1);
		StretchDownUI->GetTransform()->SetScale(Vector3(20.0f, 20.0f, 0.0f));
		UIPosition = CTransformUtils::GetBottomPosition(*FocusActorMarker, *StretchDownUI);
		StretchDownUI->GetTransform()->SetPosition(UIPosition);
		StretchDownUI->SetMouseFocusEvent([this]()->void
			{
				if (LClicked())
				{
					CLadderForm* FocusLadder = LadderEditor->GetFocusLadder();
					if (FocusLadder)
						FocusLadder->StretchToDown();
				}
			});
	}*/

	//if (LadderEditor->IsEditReady() == false)
	//	return;
	/*const int32_t MouseX = UIManager.GetMouseWorldPosition().x;
	const int32_t MouseY = UIManager.GetMouseWorldPosition().y;
	Vector2 MouseWorld2DPosition = Vector2(float(MouseX), float(MouseY));

	if (LClicked())
	{
		CLadderForm* ProximateLadder = LadderEditor->GetLadder(MouseWorld2DPosition);
		if (ProximateLadder)
		{
			LadderEditor->SetFocusLadder(ProximateLadder);
			ActorTranslator.SetFirstDiff(CMouseManager::GetInst(), *ProximateLadder);
		}
		else
		{
			CLadderForm* Ladder = LadderEditor->CreateLadder(Vector3(MouseWorld2DPosition.x, MouseWorld2DPosition.y, 1.0f));
			LadderEditor->SetFocusLadder(Ladder);
		}
	}
	else if (LHold())
	{
		CLadderForm* Ladder = LadderEditor->GetFocusLadder();
		ActorTranslator.TranslateActor(CMouseManager::GetInst(), *Ladder);
	}*/
}

void CMapEditorScene::ColliderMode()
{
	if (bPlaceGround)
	{
		TileInteractionHandler->SetGroundByHandledTiles(*GroundManager);
		bPlaceGround = false;
	}

	const int32_t MouseX = UIManager.GetMouseWorldPosition().x;
	const int32_t MouseY = UIManager.GetMouseWorldPosition().y;
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
	if (CurrentEditState)
		CurrentEditState->ExitEditState(*MainPanel);
	switch (InEditMode)
	{
	case EEditMode::Free:
		break;
	case EEditMode::Tile:
		break;
	case EEditMode::Ladder:
		CurrentEditState = LadderEditState;
		break;
	case EEditMode::Collider:
		break;
	default:
		assert(0);
		break;
	}
	EditMode = InEditMode;
	CurrentEditState->EnterEditState(*MainPanel);
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

	CurrentEditState->ToImGUI();
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