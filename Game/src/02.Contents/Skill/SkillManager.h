#pragma once
#include "01.Base/Actor/Actor.h"
#include "Skill.h"

struct TSkillManagementData
{
	TSkillData SkillData;
	std::wstring UsableImagePath;
	std::wstring UnUseableImagePath;
	std::wstring HoverImagePath;
};

class CSkillManager : public CActor
{
	GENERATE_OBJECT(CSkillManager)
public:
	CSkillManager();
	~CSkillManager() = default;

public:
	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);
	}

	void AddSkillData(uint16_t InDegree, const TSkillManagementData& InSkillManagementData)
	{
		if (InDegree >= 10)
			return;
		SkillDatas[InDegree].push_back(InSkillManagementData);
		AddedSkillDataInfos.emplace(InDegree, SkillDatas[InDegree].size() - 1);
	}

	const TSkillManagementData& TakeAddedSkillData()
	{
		auto& Pair = AddedSkillDataInfos.front();
		const TSkillManagementData& SkillData = SkillDatas[Pair.first][Pair.second];
		AddedSkillDataInfos.pop();
		return SkillData;
	}

	bool IsEmptyAddedSkillData() const { return AddedSkillDataInfos.empty(); }

	//const TSkillManagementData* GetSkillData(uint16_t InDegree, const std::string& InSkillName) const
	//{
	//	auto Iter = std::find_if(SkillDatas[InDegree].begin(), SkillDatas[InDegree].end(),
	//		[&InSkillName](const TSkillManagementData& InA) -> bool
	//		{
	//			return InA.SkillData.Name == InSkillName;
	//		});

	//	if (Iter == SkillDatas[InDegree].end())
	//		return nullptr;

	//	// iterator가 가리키는 실제 객체의 주소를 반환
	//	return &(*Iter);
	//}

private:
	std::vector<TSkillManagementData> SkillDatas[10];

	std::queue<std::pair<uint16_t, size_t>> AddedSkillDataInfos;

};

