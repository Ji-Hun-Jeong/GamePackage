#pragma once
#include "01.Base/Actor/Component/Component.h"
#include "Skill.h"

struct THitBoxData
{
	Vector2 LeftTop;
	Vector2 RightBottom;
	size_t AttackFrameNumber = 0;
};

struct TMeleeAttackData
{
	TSkillData SkillData;
	CAnimation Animation;
	THitBoxData HitBoxData;
};

class CSkillCaster : public CComponent
{
	GENERATE_OBJECT(CSkillCaster)
public:
	CSkillCaster() = default;
	~CSkillCaster() = default;

public:
	void MeleeAttack(const TMeleeAttackData& InMeleeAttackData);

private:
	// Todo: 나중에 여기서 오브젝트 풀링 구현

};

extern void DeSerialize(const rapidjson::Value& InData, THitBoxData* OutHitBoxData);