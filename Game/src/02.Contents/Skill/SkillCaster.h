#pragma once
#include "01.Base/Actor/Component/Component.h"
#include "Skill.h"

struct TWeaponAttackData
{

};

class CSkillCaster : public CComponent
{
	GENERATE_OBJECT(CSkillCaster)
public:
	CSkillCaster() = default;
	~CSkillCaster() = default;

public:
	void WeaponAttack(const std::string& InWeaponAttackName);

private:
	std::unordered_map<std::string, TWeaponAttackData> WeaponAttackDatas;

};

