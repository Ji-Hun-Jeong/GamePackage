#pragma once
#include "IEditState.h"
#include "02.Contents/Actor/Edit/LadderEditor.h"

class CLadderEditState : public IEditState
{
	GENERATE_OBJECT(CLadderEditState)
public:
	CLadderEditState();
	~CLadderEditState() = default;

public:
	void EnterEditState(TEditContext& InEditContext) override
	{
		InEditContext.MainPanel->SetMouseFocusEvent([this, &InEditContext]()->void { bBasicState = true; });
		InEditContext.MainPanel->AttachChildUI(*StretchUpUI);
		InEditContext.MainPanel->AttachChildUI(*StretchDownUI);
		StretchUpUI->Activate(false);
		StretchDownUI->Activate(false);
		LadderMarker->Activate(false);
	}
	void OnEditState(TEditContext& InEditContext) override
	{
		if (CurrentFocusLadder)
		{
			LadderMarker->GetTransform()->SetPosition(CurrentFocusLadder->GetTransform()->GetFinalPosition());
			LadderMarker->GetTransform()->SetScale(CurrentFocusLadder->GetTransform()->GetScale());

			Vector3 NewUpPosition = CTransformUtils::GetTopPosition(*CurrentFocusLadder, *StretchUpUI);
			StretchUpUI->GetTransform()->SetPosition(NewUpPosition);
			Vector3 NewDownPosition = CTransformUtils::GetBottomPosition(*CurrentFocusLadder, *StretchDownUI);
			StretchDownUI->GetTransform()->SetPosition(NewDownPosition);
		}

		if (bBasicState == false)
			return;

		if (LadderEditor->IsEditReady() == false)
			return;

		if (LClicked())
		{
			const Vector2& MouseWorld2DPosition = GetMouseWorldPosition();

			if (CLadderForm* ProximateLadder = LadderEditor->GetProximateLadder(MouseWorld2DPosition))
				CurrentFocusLadder = ProximateLadder;
			else
			{
				CLadderForm* NewLadder = LadderEditor->CreateLadder(Vector3(MouseWorld2DPosition.x, MouseWorld2DPosition.y, 1.0f));
				CurrentFocusLadder = NewLadder;
			}
			ActorTranslator.SetFirstDiff(CMouseManager::GetInst(), CurrentFocusLadder->GetTransform()->GetPosition());
			LadderMarker->Activate(true);
			LadderMarker->GetSpriteRenderComponent()->SetColor(Vector3(1.0f, 0.0f, 0.0f), 1.0f);

			bFocusLadderMove = true;
		}
		else if (LHold())
		{
			if (bFocusLadderMove == false)
				return;

			ActorTranslator.TranslateActor(CMouseManager::GetInst(), *CurrentFocusLadder);
			LadderMarker->GetTransform()->SetPosition(CurrentFocusLadder->GetTransform()->GetFinalPosition());
		}
		else if (LReleased())
		{
			if (CurrentFocusLadder == nullptr)
				return;

			StretchUpUI->Activate(true);
			StretchDownUI->Activate(true);

			bFocusLadderMove = false;
		}
		else if (RHold())
		{
			CurrentFocusLadder->Activate(false);
			CurrentFocusLadder = nullptr;
			StretchUpUI->Activate(false);
			StretchDownUI->Activate(false);
		}

		bBasicState = false;

	}
	void ExitEditState(TEditContext& InEditContext) override
	{
		InEditContext.MainPanel->SetMouseFocusEvent(nullptr);
		InEditContext.MainPanel->DetachChildUI(*StretchUpUI);
		InEditContext.MainPanel->DetachChildUI(*StretchDownUI);
		StretchUpUI->Activate(false);
		StretchDownUI->Activate(false);
		LadderMarker->Activate(false);
	}
	void ToImGUI() override
	{
		if (ImGui::Button("SetLadderHead"))
		{
			std::wstring LadderHeadImagePath;
			if (CWindowManager::GetInst().TryGetFilePathByDialog(&LadderHeadImagePath))
				LadderEditor->SetHeadImagePath(LadderHeadImagePath);
		}
		if (ImGui::Button("SetLadderBody"))
		{
			std::wstring LadderBodyImagePath;
			if (CWindowManager::GetInst().TryGetFilePathByDialog(&LadderBodyImagePath))
				LadderEditor->AddBodyImagePath(LadderBodyImagePath);
		}
		if (ImGui::Button("SetLadderFoot"))
		{
			std::wstring LadderFootImagePath;
			if (CWindowManager::GetInst().TryGetFilePathByDialog(&LadderFootImagePath))
				LadderEditor->SetFootImagePath(LadderFootImagePath);
		}
	}

private:
	bool bBasicState = false;

	CLadderEditor* LadderEditor = nullptr;
	bool bCreateLadder = false;

	CActorTranslator ActorTranslator;

	CUI* StretchUpUI = nullptr;
	CUI* StretchDownUI = nullptr;
	bool bStretchUp = false;
	bool bStretchDown = false;

	CLadderForm* CurrentFocusLadder = nullptr;
	CStaticActor* LadderMarker = nullptr;

	bool bFocusLadderMove = false;

};

