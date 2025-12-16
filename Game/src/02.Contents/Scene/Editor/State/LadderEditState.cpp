#include "pch.h"
#include "LadderEditState.h"

#include "GameCore.h"
CLadderEditState::CLadderEditState()
{
	LadderEditor = GetWorld()->SpawnActor<CLadderEditor>(this);

	LadderEditor->SetHeadImagePath(L"resources/image/Tile/ladder/ladder0.png");
	LadderEditor->AddBodyImagePath(L"resources/image/Tile/ladder/ladder1.png");
	LadderEditor->AddBodyImagePath(L"resources/image/Tile/ladder/ladder2.png");
	LadderEditor->SetFootImagePath(L"resources/image/Tile/ladder/ladder3.png");

	LadderMarker = GetWorld()->SpawnActor<CStaticActor>(this);
	LadderMarker->SetLineActor();
	LadderMarker->Activate(false);

	StretchUpUI = GetWorld()->SpawnActor<CUI>(this);
	StretchUpUI->SetLineActor();
	StretchUpUI->GetTransform()->SetScale(Vector3(20.0f, 20.0f, 1.0f));
	StretchUpUI->Activate(false);
	StretchUpUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
			{
				LadderEditor->StretchToUp(*CurrentFocusLadder);
				Vector3 NewUpPosition = CTransformUtils::GetTopPosition(*CurrentFocusLadder, *StretchUpUI);
				StretchUpUI->GetTransform()->SetPosition(NewUpPosition);
			}
		});

	StretchDownUI = GetWorld()->SpawnActor<CUI>(this);
	StretchDownUI->SetLineActor();
	StretchDownUI->GetTransform()->SetScale(Vector3(20.0f, 20.0f, 1.0f));
	StretchDownUI->Activate(false);
	StretchDownUI->SetMouseFocusEvent([this]()->void
		{
			if (LClicked())
			{
				LadderEditor->StretchToDown(*CurrentFocusLadder);
				Vector3 NewDownPosition = CTransformUtils::GetBottomPosition(*CurrentFocusLadder, *StretchDownUI);
				StretchDownUI->GetTransform()->SetPosition(NewDownPosition);
			}
		});
}