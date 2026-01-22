#pragma once
#include "SkillCaster.h"
#include "Common/Json.h"
#include "02.Contents/Wz/WzLoader.h"

class CSkillLoader
{
public:
	CSkillLoader() = default;
	~CSkillLoader() = default;

public:
	const TMeleeAttackData* GetMeleeAttackData(const std::string& InSkillName) const
	{
		auto Iter = MeleeAttackDatas.find(InSkillName);
		if (Iter == MeleeAttackDatas.end())
			return nullptr;
		return &Iter->second;
	}

	void LoadSkillData(CWzLoader& InWzLoader, const std::string_view InWzJsonPath)
	{
		InWzLoader.OpenWzData(InWzJsonPath);

		const auto& Document = InWzLoader.GetLoadData();
		if (Document.HasMember("dir") == false)
			return;
		auto& Value = Document["dir"];
		

		InWzLoader.CloseWzData();
	}

private:
	std::map<std::string, TMeleeAttackData> MeleeAttackDatas;

};
