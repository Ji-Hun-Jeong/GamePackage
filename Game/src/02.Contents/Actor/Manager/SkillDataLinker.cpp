#include "pch.h"
#include "SkillDataLinker.h"

#include "GameCore.h"


void CSkillDataLinker::SetupUI(CUIManager& InUIManager)
{
	/*SkillManagementUI = GetWorld()->SpawnActor<CUI>(this);
	SkillManagementUI->SetUIScale(Vector2(50.0f, 50.0f));
	SkillManagementUI->SetMouseClickEvent([this]()->void
		{
			if (SkillManager == nullptr || PlayerController == nullptr)
				return;

			const TSkillManagementData* SkillData = SkillManager->GetSkillData(0, "");
			if (SkillData == nullptr)
				return;

			PlayerController->AddKeyInput(EKeyType::A, [SkillData]()->void
				{
					std::cout << SkillData->SkillData.Description << '\n';
				});
		});

	KeyBoardUI = GetWorld()->SpawnActor<CUI>(this);
	KeyBoardUI->GetTransform()->SetPosition(Vector3(50.0f, 0.0f, 0.0f));
	KeyBoardUI->SetUIScale(Vector2(50.0f, 50.0f));

	InUIManager.AddUI(*SkillManagementUI);
	InUIManager.AddUI(*KeyBoardUI);*/
}
