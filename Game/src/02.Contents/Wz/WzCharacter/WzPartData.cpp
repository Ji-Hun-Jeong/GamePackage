#include "pch.h"
#include "WzPartData.h"

#include "Utils.h"

namespace Wz
{
	EWzPartType GetPartTypeByName(const std::string_view InPartName)
	{
		if (InPartName == "body")			return EWzPartType::Body;
		if (InPartName == "arm")			return EWzPartType::Arm;
		if (InPartName == "hand")			return EWzPartType::Hand;
		if (InPartName == "armOverHair")	return EWzPartType::ArmOverHair;
		if (InPartName == "lHand")			return EWzPartType::LHand;
		if (InPartName == "rHand")			return EWzPartType::RHand;
		if (InPartName == "head")			return EWzPartType::Head;
		if (InPartName == "humanEar")		return EWzPartType::HumanEar;
		return EWzPartType::End;
	}

	const TWzPartData* GetFinalPartData(const TWzPartData& InPartData)
	{
		const TWzPartData* FinalPartData = &InPartData;
		while (FinalPartData->RefPartData)
			FinalPartData = FinalPartData->RefPartData;
		return FinalPartData;
	}

	const TWzFrameData* GetFinalFrameData(const TWzFrameData& InFrameData)
	{
		const TWzFrameData* FinalPartData = &InFrameData;
		while (FinalPartData->RefFrame)
			FinalPartData = FinalPartData->RefFrame;
		return FinalPartData;
	}

}

