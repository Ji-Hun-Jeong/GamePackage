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

struct TWzPartData : public TWzPng
{
	std::string Uol;
	TWzMap Map;
	std::string Z;
	std::string Group;
	const TWzPartData* RefPartData = nullptr;
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