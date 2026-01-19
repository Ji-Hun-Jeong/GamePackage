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

			DeSerialize(SkillData, &MeleeAttackData.SkillData);

			if (SkillData.HasMember("Animation") && SkillData["Animation"].IsObject())
				MeleeAttackData.Animation.DeSerialize(SkillData["Animation"]);

			DeSerialize(SkillData, &MeleeAttackData.HitBoxData);

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

