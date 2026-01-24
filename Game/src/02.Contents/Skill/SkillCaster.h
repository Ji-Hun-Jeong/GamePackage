#pragma once
#include "01.Base/Actor/Component/Component.h"
#include "Skill.h"

class CSkillCaster : public CComponent
{
	GENERATE_OBJECT(CSkillCaster)
public:
	CSkillCaster() = default;
	~CSkillCaster() = default;

public:
	void Update(float InDeltaTime)
	{

	}
	void CastInstantSkill(const TSkillData& InSkillData);

private:

};
