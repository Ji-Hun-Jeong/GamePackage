#pragma once
#include "01.Base/Actor/Component/Component.h"
#include "01.Base/Actor/StaticActor.h"

enum class EWzPartType
{
	Body,
	Arm,
	Hand,
	ArmOverHair,
	LHand,
	RHand,
	End,
};

struct TWzPartData
{
	std::string Value;
	Vector2 Origin;
	struct TMap
	{
		Vector2 Neck;
		Vector2 Navel;
		Vector2 Hand;
	} Map;
	std::string Z;
	std::string Group;
	std::string OutLink;
};

class CWzPart : public CStaticActor
{
	GENERATE_OBJECT(CWzPart)
public:
	CWzPart()
		: PartType(EWzPartType::End)
	{
	}
	~CWzPart() = default;

public:
	void SetPartType(EWzPartType InPartType) { PartType = InPartType; }
	EWzPartType GetPartType() const { return PartType; }

	void SetPartData(const TWzPartData& InPartData) { PartData = InPartData; }
	TWzPartData& GetPartData() { return PartData; }

	static EWzPartType GetPartTypeByName(const std::string& InPartName);

private:
	EWzPartType PartType;

	TWzPartData PartData;

};

class CWzFrameData
{
public:
	CWzFrameData() = default;
	~CWzFrameData() = default;

public:
	TWzPartData& GetPartDataRef(EWzPartType InPartType)
	{
		return PartDatas[static_cast<size_t>(InPartType)];
	}
	const TWzPartData& GetPartData(EWzPartType InPartType) const
	{
		return PartDatas[static_cast<size_t>(InPartType)];
	}

	void SetAction(const std::string& InAction) { Action = InAction; }
	void SetFace(int16_t InFace) { Face = InFace; }
	void SetDelay(int32_t InDelay) { Delay = InDelay; }
	void SetFrame(int32_t InFrame) { Frame = InFrame; }
	void SetMove(const Vector2& InMove) { Move = InMove; }

private:
	TWzPartData PartDatas[static_cast<size_t>(EWzPartType::End)];

	std::string Action;
	int16_t Face = 0;
	int32_t Delay = 0;
	int32_t Frame = 0;
	Vector2 Move;

};

class CWzCharacterAnimation
{
	friend class CWzCharacterAnimator;
public:
	CWzCharacterAnimation(const std::string& InAnimName) : AnimName(InAnimName) {}
	~CWzCharacterAnimation() = default;

public:
	void AddFrameData(const CWzFrameData& InFrameData)
	{
		Frames.push_back(InFrameData);
	}

private:
	std::string AnimName;
	std::vector<CWzFrameData> Frames;

};

class CWzCharacterAnimator : public CComponent
{
	GENERATE_OBJECT(CWzCharacterAnimator)
public:
	CWzCharacterAnimator();
	~CWzCharacterAnimator() = default;

public:
	void PlayAnimation(const std::string& InAnimName)
	{
		auto Iter = WzCharacterAnimations.find(InAnimName);
		if (Iter == WzCharacterAnimations.end())
		{
			std::cout << "[CWzCharacterAnimator::PlayAnimation] 애니메이션을 찾을 수 없습니다: " << InAnimName << std::endl;
			return;
		}
		CWzCharacterAnimation& Animation = Iter->second;
		const std::vector<CWzFrameData>& Frames = Animation.Frames;
		for (auto& Frame : Frames)
		{
			// 각 파트에 FrameData를 적용
			for (size_t PartIndex = 0; PartIndex < static_cast<size_t>(EWzPartType::End); ++PartIndex)
			{
				EWzPartType PartType = static_cast<EWzPartType>(PartIndex);
				
				const TWzPartData& PartData = Frame.GetPartData(PartType);
				
				CWzPart* Part = GetPart(PartType);
				Part->SetPartData(PartData);
			}
		}
	}
	CWzCharacterAnimation& GetAnimation(const std::string& InAnimName)
	{
		if (WzCharacterAnimations.contains(InAnimName) == false)
			WzCharacterAnimations.emplace(InAnimName, InAnimName);
		return WzCharacterAnimations.at(InAnimName);
	}
	CWzPart* GetPart(EWzPartType InPartType)
	{
		if (InPartType == EWzPartType::End)
		{
			std::cout << "[CWzCharacterAnimator::GetPart] 잘못된 파트 타입 요청: End" << std::endl;
			return nullptr;
		}
		return Parts[static_cast<size_t>(InPartType)];
	}

private:
	CWzPart* Parts[static_cast<size_t>(EWzPartType::End)];

	std::unordered_map<std::string, CWzCharacterAnimation> WzCharacterAnimations;

};

