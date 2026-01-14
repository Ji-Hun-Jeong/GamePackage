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

private:
	EWzPartType PartType;

	TWzPartData PartData;

};

class CWzCharacterFrame
{
public:

private:

};
class CWzCharacterAnimation
{
public:
	CWzCharacterAnimation() = default;
	~CWzCharacterAnimation() = default;

public:

private:

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
	}

private:
	CWzPart* PartActors[static_cast<size_t>(EWzPartType::End)];

	std::unordered_map<std::string, CWzCharacterAnimation> WzCharacterAnimations;
};

