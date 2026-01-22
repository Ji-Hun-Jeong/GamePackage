#pragma once
#include "WzCharacterAnimation.h"

class CWzPartsManager
{
public:
	explicit CWzPartsManager(std::array<CWzPart*, static_cast<size_t>(EWzPartType::End)>&& InParts);
	~CWzPartsManager() = default;

public:
	// 얘가 직접 만들어줘야할듯?
	void UpdateParts();
	void CompositeParts(const CWzFrameData& InFrameData);
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