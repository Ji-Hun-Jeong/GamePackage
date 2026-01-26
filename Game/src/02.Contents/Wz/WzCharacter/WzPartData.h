#pragma once
#include "02.Contents/Wz/WzBase.h"

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

struct TWzMap : public CWzNode
{
	Vector2 Neck;
	Vector2 Navel;
	Vector2 Hand;
	Vector2 HandMove;
};

struct TWzPartData : public CWzNode
{
	TWzMap Map;
	Vector2 Origin;
	std::string Z;
	std::string Group;
	std::wstring OutLink;
	const TWzPartData* Uol = nullptr;
};

struct TWzCharacterFrameData : public CWzNode
{
	std::map<EWzPartType, TWzPartData> PartDatas;
	std::string Action;
	int16_t Face = 0;
	int32_t Delay = 0;
	int32_t Frame = 0;
	int32_t Move = 0;
	const TWzCharacterFrameData* RefFrame = nullptr;
};

namespace Wz
{
	extern EWzPartType GetPartTypeByName(const std::string_view InPartName);
	extern const TWzPartData* GetFinalPartData(const TWzPartData& InPartData);
	extern const TWzCharacterFrameData* GetFinalFrameData(const TWzCharacterFrameData& InFrameData);
}