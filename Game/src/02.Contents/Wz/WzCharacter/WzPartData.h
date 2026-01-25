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
	std::string Uol;
	TWzMap Map;
	Vector2 Origin;
	std::string Z;
	std::string Group;
	std::wstring OutLink;
};

struct TWzCharacterFrameData : public CWzNode
{
	std::map<EWzPartType, TWzPartData> PartDatas;
	std::string Action;
	int16_t Face = 0;
	int32_t Delay = 0;
	int32_t Frame = 0;
	int32_t Move = 0;
};

namespace Wz
{
	extern EWzPartType GetPartTypeByName(const std::string_view InPartName);
}