#include "pch.h"
#include "SkillManager.h"

#include "GameCore.h"
#include "02.Contents/Actor/UI/UIManager.h"

void CSkillManager::LinkToUI(CUIManager& InUIManager)
{
	CUI* ManagementUI = GetWorld()->SpawnActor<CUI>(this);
	ManagementUI->GetTransform()->SetScale(Vector3(50.0f, 50.0f, 0.0f));
	ManagementUI->GetSpriteRenderComponent()->SetColor(Vector3(1.0f, 0.0f, 0.0f), 1.0f);
	ManagementUI->SetMouseFocusEvent([]()->void
		{
			if (LClicked())
				std::cout << "Hi\n";
		});
	InUIManager.AddUI(*ManagementUI);
}
