#pragma once
#include "01.Base/Actor/StaticActor.h"
#include "WzPartData.h"

class CWzPart : public CStaticActor
{
	GENERATE_OBJECT(CWzPart)
public:
	CWzPart() = default;
	~CWzPart() = default;

public:

private:

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
	CWzPart* GetPart(EWzPartType InPartType)
	{
		if (InPartType >= EWzPartType::End)
			return nullptr;
		return Parts[static_cast<size_t>(InPartType)];
	}

private:
	std::array<CWzPart*, static_cast<size_t>(EWzPartType::End)> Parts;

};