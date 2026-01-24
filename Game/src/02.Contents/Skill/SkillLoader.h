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

	void LoadSkillData(CWzLoader& InWzLoader, const std::string_view InWzJsonPath)
	{
		InWzLoader.OpenWzData(InWzJsonPath);

		const auto& Document = InWzLoader.GetLoadData();
		if (Document.HasMember("dir") == false)
			return;
		auto& Value = Document["dir"];
		

		InWzLoader.CloseWzData();
	}

};
