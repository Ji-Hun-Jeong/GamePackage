#pragma once
#include "01.Base/Actor/StaticActor.h"
#include "WzPartData.h"

class CWzPart : public CStaticActor
{
	GENERATE_OBJECT(CWzPart)
public:
	CWzPart() : PartType(EWzPartType::End) {}
	~CWzPart() = default;

public:
	void Composite();
	void SetPartType(EWzPartType InPartType) { PartType = InPartType; }
	EWzPartType GetPartType() const { return PartType; }

	void AttachChildPart(CWzPart& InChildPart)
	{
		InChildPart.OwnerPart = this;
		ChildParts.push_back(&InChildPart);
	}

private:
	EWzPartType PartType;

	CWzPart* OwnerPart = nullptr;
	std::vector<CWzPart*> ChildParts;

	Vector2 FinalNeck;
	Vector2 FinalNavel;
	Vector2 FinalHand;
	Vector2 FinalHandMove;

};

class CWzPartsManager : public CComponent
{
	GENERATE_OBJECT(CWzPartsManager)
public:
	CWzPartsManager() = default;
	~CWzPartsManager() = default;

public:
	void InitalizeComponent() override;
	void CompositeParts(const TWzCharacterFrameData& InFrameData);
	CWzPart* GetPart(EWzPartType InPartType) const
	{
		if (InPartType == EWzPartType::End)
		{
			std::cout << "[CWzCharacterAnimator::GetPart] 잘못된 파트 타입 요청: End" << std::endl;
			return nullptr;
		}
		return Parts[static_cast<size_t>(InPartType)];
	}

private:
	std::array<CWzPart*, static_cast<size_t>(EWzPartType::End)> Parts;

};