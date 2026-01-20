#include "pch.h"
#include "SkillManager.h"

CSkillManager::CSkillManager()
{
	AddSkillData(0, {});
	SkillDatas[0][0].UsableImagePath = L"resources/image/Player/Alert/0.png";
	SkillDatas[0][0].SkillData.Description = "Hisdfsdf";
}
