#pragma once
#include "01.Base/Actor/Actor.h"
#include "Skill.h"

struct TSkillManagementData
{
	TSkillData SkillData;
	std::wstring UsableImagePath;
	std::wstring UnUseableImagePath;
	std::wstring HoverImagePath;
};

class CSkillManager : public CActor
{
	GENERATE_OBJECT(CSkillManager)
public:
	CSkillManager() = default;
	~CSkillManager() = default;

public:
	void AddSkillData(uint16_t InDegree, const TSkillManagementData& InSkillManagementData)
	{
		if (InDegree >= 10)
			return;
		SkillDatas[InDegree].push_back(InSkillManagementData);
	}
	void LinkToUI(class CUIManager& InUIManager);

private:
	std::vector<TSkillManagementData> SkillDatas[10];

};

