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
				{
					if (LadderEditor->IsEditReady() == false)
						return;
					const Vector2& MouseWorld2DPosition = GetMouseWorldPosition();
					CLadderForm* Ladder = LadderEditor->CreateLadder(Vector3(MouseWorld2DPosition.x, MouseWorld2DPosition.y, 1.0f));
					LadderEditor->SetFocusLadder(Ladder);
				}
			});
	}
	void OnEditState(CUI& InMainPanel) override
	{
		if (bSetLadderHead)
		{
			std::wstring LadderHeadImagePath;
			if (CWindowManager::GetInst().TryGetFilePathByDialog(&LadderHeadImagePath))
				LadderEditor->SetHeadImagePath(LadderHeadImagePath);
			bSetLadderHead = false;
		}
		if (bSetLadderBody)
		{
			std::wstring LadderBodyImagePath;
			if (CWindowManager::GetInst().TryGetFilePathByDialog(&LadderBodyImagePath))
				LadderEditor->AddBodyImagePath(LadderBodyImagePath);
			bSetLadderBody = false;
		}
		if (bSetLadderFoot)
		{
			std::wstring LadderFootImagePath;
			if (CWindowManager::GetInst().TryGetFilePathByDialog(&LadderFootImagePath))
				LadderEditor->SetFootImagePath(LadderFootImagePath);
			bSetLadderFoot = false;
		}

		LadderEditor->InteractionToScreen(InMainPanel, []()->void
			{
				static int i = 0;
				//std::cout << "Hi "<< i++ << std::endl;
				/*if (LClicked())
					std::cout << "Hi\n";*/
			});
	}
	void ExitEditState(CUI& InMainPanel) override
	{
		InMainPanel.SetMouseFocusEvent(nullptr);
	}
	void ToImGUI() override
	{
		if (ImGui::Button("SetLadderHead"))
			bSetLadderHead = true;
		if (ImGui::Button("SetLadderBody"))
			bSetLadderBody = true;
		if (ImGui::Button("SetLadderFoot"))
			bSetLadderFoot = true;
	}

private:
	CLadderEditor* LadderEditor = nullptr;
	bool bSetLadderHead = false;
	bool bSetLadderBody = false;
	bool bSetLadderFoot = false;

};

