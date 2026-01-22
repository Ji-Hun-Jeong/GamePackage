#pragma once
#include "01.Base/Actor/StaticActor.h"

using JValue = rapidjson::Value;
using JObject = rapidjson::GenericObject<true, JValue>;
struct TSkillNode
{
	std::string Value;
	std::map<std::string, TSkillNode*> Members;
};

struct TSkillPng : public TSkillNode
{
	Vector2 Origin;
	int32_t Z = 0;
	int32_t Delay = 0;
	std::string OutLink;
};

struct TSkillCommon : public TSkillNode
{
	int32_t MaxLavel = 0;
	std::string MpCon;
	std::string MobCount;
	std::string Damage;
	std::string AttackCount;
	Vector2 LeftTop;
	Vector2 RightBottom;
};

struct TSkillInfo : public TSkillNode
{
	int32_t Type = 0;
	int32_t AreaAttack = 0;
};

struct TSkillHit : public TSkillNode
{
	std::vector<TSkillPng> Anim;
};

struct TSkillEffect : public TSkillNode
{
	std::vector<TSkillPng> Anim;
};

struct TSkillData : public TSkillNode
{
	TSkillPng Icon;
	TSkillPng IconMouseOver;
	TSkillPng IconDisabled;

	TSkillCommon Common;
	std::vector<TSkillHit> Hit;
	TSkillEffect Effect;
	TSkillInfo Info;

	int32_t Weapon = 0;
	int32_t SubWeapon = 0;
	std::string Action;
};

extern bool DeSerializeSkillData(const rapidjson::Value& InValue, const std::string_view InSkillCode
	, TSkillData* OutSkillData);
