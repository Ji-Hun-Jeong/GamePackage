#pragma once
#include "01.Base/Manager/WzLoader.h"

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
		Vector2 HandMove;
	} Map;
	std::string Z;
	std::string Group;
	std::string OutLink;
};

extern bool ParseVectorObject(const rapidjson::GenericObject<true, rapidjson::Value>& InVectorObject
	, Vector2* OutWzVector);
extern bool ParsePartPngObject(const rapidjson::GenericObject<true, rapidjson::Value>& InPngObject
	, TWzPartData* OutPartData);
extern bool ParseStringData(const rapidjson::Value& InStringValue, TWzPartData* OutPartData);
extern bool ParseMapData(const rapidjson::Value& InVectorValue, TWzPartData* OutPartData);