#pragma once
#include "01.Base/Actor/Actor.h"

#include "02.Contents/Actor/UI/UIManager.h"
#include "02.Contents/Actor/Player/PlayerController.h"
#include "02.Contents/Skill/SkillManager.h"

class CSkillDataLinker : public CActor
{
	GENERATE_OBJECT(CSkillDataLinker)
public:
	CSkillDataLinker() = default;
	~CSkillDataLinker() = default;

public:
	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);

		while (SkillManager->IsEmptyAddedSkillData() == false)
		{
			const TSkillManagementData& SkillManagementData = SkillManager->TakeAddedSkillData();
			// UIÃß°¡
		}
	}
	void SetupUI(CUIManager& InUIManager);
	void SetPlayerController(CPlayerController& InPlayerController) {PlayerController = &InPlayerController;}
	void SetSkillManager(CSkillManager& InSkillManager) { SkillManager = &InSkillManager; }

private:
	CPlayerController* PlayerController = nullptr;
	CSkillManager* SkillManager = nullptr;

	CUI* SkillManagementUI = nullptr;
	CUI* KeyBoardUI = nullptr;

};

