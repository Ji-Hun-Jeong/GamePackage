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
		InEditContext.MainPanel->SetMouseFocusEvent([this, &InEditContext]()->void
			{
				if (LClicked())
					CreateLadder(InEditContext);
			});
		StretchUpUI->SetMouseFocusEvent([this]()->void
			{
				if (LClicked())
					bStretchUp = true;
			});
		StretchUpUI->GetTransform()->SetPosition(Vector3(0.0f, 20.0f, 1.0f));
		StretchUpUI->GetTransform()->SetScale(Vector3(20.0f, 20.0f, 1.0f));
		StretchUpUI->Activate(false);

		StretchDownUI->SetMouseFocusEvent([this]()->void
			{
				if (LClicked())
					bStretchDown = true;
			});
		StretchDownUI->GetTransform()->SetPosition(Vector3(0.0f, -20.0f, 1.0f));
		StretchDownUI->GetTransform()->SetScale(Vector3(20.0f, 20.0f, 1.0f));
		StretchDownUI->Activate(false);
	}
	void OnEditState(TEditContext& InEditContext) override
	{
		if (CLadderForm* FocusLadder = LadderEditor->GetFocusLadder())
		{
			LadderMarker->GetTransform()->SetPosition(FocusLadder->GetTransform()->GetFinalPosition());
			LadderMarker->GetTransform()->SetScale(FocusLadder->GetTransform()->GetScale());
			LadderMarker->GetSpriteRenderComponent()->SetColor(Vector3(1.0f, 0.0f, 0.0f), 1.0f);

			StretchUpUI->Activate(true);
			StretchDownUI->Activate(true);
			FocusLadder->AttachChildUI(*StretchUpUI);
			FocusLadder->AttachChildUI(*StretchDownUI);

			if (LHold())
			{
				if (CurrentMoveLadder)
					ActorTranslator.TranslateActor(CMouseManager::GetInst(), *CurrentMoveLadder);
			}
			else if (LReleased())
				CurrentMoveLadder = nullptr;

			if (bStretchUp)
			{
				LadderEditor->StretchToUp(*FocusLadder);
				bStretchUp = false;
			}
			else if (bStretchDown)
			{
				LadderEditor->StretchToDown(*FocusLadder);
				bStretchDown = false;
			}
		}
	}
	void ExitEditState(TEditContext& InEditContext) override
	{
		InEditContext.MainPanel->SetMouseFocusEvent(nullptr);
		StretchUpUI->SetMouseFocusEvent(nullptr);
		StretchUpUI->Activate(false);
		StretchDownUI->SetMouseFocusEvent(nullptr);
		StretchDownUI->Activate(false);
		LadderEditor->DetachToPanel(*InEditContext.MainPanel);
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
	void CreateLadder(TEditContext& InEditContext)
	{
		if (LadderEditor->IsEditReady() == false)
			return;

		const Vector2& MouseWorld2DPosition = GetMouseWorldPosition();
		CLadderForm* Ladder = LadderEditor->CreateLadder(Vector3(MouseWorld2DPosition.x, MouseWorld2DPosition.y, 1.0f));
		LadderEditor->SetFocusLadder(Ladder);

		Ladder->SetMouseFocusEvent([this, Ladder]()->void
			{
				if (LClicked())
				{
					CurrentMoveLadder = Ladder;
					ActorTranslator.SetFirstDiff(CMouseManager::GetInst(), *CurrentMoveLadder);
					LadderEditor->SetFocusLadder(Ladder);
				}
			});
		LadderEditor->AttachToPanel(*InEditContext.MainPanel);
	}

private:
	CLadderEditor* LadderEditor = nullptr;
	bool bCreateLadder = false;

	CStaticActor* LadderMarker = nullptr;

	CActorTranslator ActorTranslator;

	CLadderForm* CurrentMoveLadder = nullptr;

	CActor* StretchUI = nullptr;
	CUI* StretchUpUI = nullptr;
	CUI* StretchDownUI = nullptr;
	bool bStretchUp = false;
	bool bStretchDown = false;
};

