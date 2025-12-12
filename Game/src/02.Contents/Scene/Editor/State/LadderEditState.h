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
	void EnterEditState(CUI& InMainPanel) override
	{
		InMainPanel.SetMouseFocusEvent([this]()->void
			{
				if (LClicked())
					bCreateLadder = true;
			});
	}
	void OnEditState(CUI& InMainPanel) override
	{
		if (bCreateLadder)
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
			bCreateLadder = false;
		}
		if (CLadderForm* FocusLadder = LadderEditor->GetFocusLadder())
		{
			LadderMarker->GetTransform()->SetPosition(InMainPanel.GetTransform()->GetFinalPosition() + FocusLadder->GetTransform()->GetPosition());
			LadderMarker->GetTransform()->SetScale(FocusLadder->GetTransform()->GetScale());
			LadderMarker->GetSpriteRenderComponent()->SetColor(Vector3(1.0f, 0.0f, 0.0f), 1.0f);

			if (LHold())
			{
				if (CurrentMoveLadder)
					ActorTranslator.TranslateActor(CMouseManager::GetInst(), *CurrentMoveLadder);
			}
		}

		LadderEditor->InteractionToScreen(InMainPanel);
	}
	void ExitEditState(CUI& InMainPanel) override
	{
		InMainPanel.SetMouseFocusEvent(nullptr);
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
	CLadderEditor* LadderEditor = nullptr;
	bool bCreateLadder = false;

	CStaticActor* LadderMarker = nullptr;

	CActorTranslator ActorTranslator;

	CLadderForm* CurrentMoveLadder = nullptr;
};

