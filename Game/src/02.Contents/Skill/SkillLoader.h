#pragma once
#include "SkillCaster.h"
#include "Common/Json.h"
#include "01.Base/Manager/WzLoader.h"

class CSkillLoader
{
public:
	CSkillLoader() = default;
	~CSkillLoader() = default;

public:
	void LoadMeleeAttackData(CWzLoader& InWzLoader, const std::string& InDataPath)
	{
		InWzLoader.OpenWzData(InDataPath);

		const rapidjson::Document& LoadData = InWzLoader.GetLoadData();

		auto LoadObject = LoadData.GetObject();
		for (auto Iter = LoadObject.MemberBegin(); Iter != LoadObject.MemberEnd(); ++Iter)
		{
			const auto& SkillNumberData = Iter->name;
			const auto& SkillData = Iter->value;

			TMeleeAttackData MeleeAttackData;

			if (SkillData.HasMember("SkillName") && SkillData["SkillName"].IsString())
				MeleeAttackData.SkillData.Name = SkillData["SkillName"].GetString();

			if (SkillData.HasMember("SkillType") && SkillData["SkillType"].IsInt())
				MeleeAttackData.SkillData.SkillType = static_cast<ESkillType>(SkillData["SkillType"].GetInt());

			if (SkillData.HasMember("Description") && SkillData["Description"].IsString())
				MeleeAttackData.SkillData.Description = SkillData["Description"].GetString();

			if (SkillData.HasMember("Animation") && SkillData["Animation"].IsObject())
				MeleeAttackData.Animation.Parse(SkillData["Animation"]);

			if (SkillData.HasMember("HitBox") && SkillData["HitBox"].IsObject())
			{
				auto HitBoxData = SkillData["HitBox"].GetObject();
				auto LeftTop = HitBoxData["LeftTop"].GetArray();
				auto RightBottom = HitBoxData["RightBottom"].GetArray();
				MeleeAttackData.HitBoxData.LeftTop = Vector2(LeftTop[0].GetFloat(), LeftTop[1].GetFloat());
				MeleeAttackData.HitBoxData.RightBottom = Vector2(RightBottom[0].GetFloat(), RightBottom[1].GetFloat());
				MeleeAttackData.HitBoxData.AttackFrameNumber = HitBoxData["AttackFrameNumber"].GetInt();
			}

			MeleeAttackDatas.emplace(MeleeAttackData.SkillData.Name, MeleeAttackData);
		}

		InWzLoader.CloseWzData();
	}

	const TMeleeAttackData* GetMeleeAttackData(const std::string& InSkillName) const
	{
		auto Iter = MeleeAttackDatas.find(InSkillName);
		if (Iter == MeleeAttackDatas.end())
			return nullptr;
		return &Iter->second;
	}

private:
	std::map<std::string, TMeleeAttackData> MeleeAttackDatas;

};

