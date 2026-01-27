#pragma once
#include "01.Base/Actor/StaticActor.h"
#include "02.Contents/Wz/WzBase.h"

struct TSkillCommon : public CWzNode
{
	int32_t MaxLavel = 0;
	std::string MpCon;
	std::string MobCount;
	std::string Damage;
	std::string AttackCount;
	Vector2 LeftTop;
	Vector2 RightBottom;
};

struct TSkillInfo : public CWzNode
{
	int32_t Type = 0;
	int32_t AreaAttack = 0;
};

struct TWzSkillPng : public TWzPng
{
	int32_t Z = 0;
	int32_t Delay = 0;
};

struct TSkillHit : public CWzNode
{
	std::vector<TWzSkillPng> Anim;
};

struct TSkillEffect : public CWzNode
{
	std::vector<TWzSkillPng> Anim;
};

struct TSkillData : public CWzNode
{
	TWzSkillPng Icon;
	TWzSkillPng IconMouseOver;
	TWzSkillPng IconDisabled;

	TSkillCommon Common;
	std::vector<TSkillHit> Hit;
	TSkillEffect Effect;
	TSkillInfo Info;

	int32_t Weapon = 0;
	int32_t SubWeapon = 0;
};

namespace SkillLoad
{
	extern bool DeSerializeSkillData(const rapidjson::Value& InValue, const std::string_view InSkillCode
		, TSkillData* OutSkillData);
}
