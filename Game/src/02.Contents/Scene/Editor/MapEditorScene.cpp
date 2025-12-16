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

	LadderEditState = GetWorld()->SpawnActor<CLadderEditState>(this);
	TileEditState = GetWorld()->SpawnActor<CTileEditState>(this);

	EditContext.MainPanel = GetWorld()->SpawnActor<CUI>(this);
	EditContext.MainPanel->GetTransform()->SetScale(Vector3(1270.0f, 950.0f, 1.0f));
	EditContext.TileMap = GetWorld()->SpawnActor<CTileMap>(this);

	ChangeMode(EditMode);

	UIManager.AddUI(*EditContext.MainPanel);
}

void CMapEditorScene::Update(float InDeltaTime)
{
	CScene::Update(InDeltaTime);

	const int32_t MouseX = UIManager.GetMouseWorldPosition().x;
	const int32_t MouseY = UIManager.GetMouseWorldPosition().y;
	Vector2 MouseWorld2DPosition = Vector2(float(MouseX), float(MouseY));
	CurrentEditState->SetMouseWorldPosition(MouseWorld2DPosition);
	CurrentEditState->OnEditState(EditContext);

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
	/*if (bPlaceGround)
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
	}*/
}

void CMapEditorScene::ChangeMode(EEditMode InEditMode)
{
	if (CurrentEditState)
		CurrentEditState->ExitEditState(EditContext);
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
		break;
	default:
		assert(0);
		break;
	}
	EditMode = InEditMode;
	CurrentEditState->EnterEditState(EditContext);
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