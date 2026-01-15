#include "pch.h"
#include "WzCharacterAnimator.h"

#include "GameCore.h"
#include "WzCharacterLoader.h"

CWzCharacterAnimator::CWzCharacterAnimator()
{
	for (size_t i = 0; i < static_cast<size_t>(EWzPartType::End); ++i)
	{
		CWzPart* Part= GetWorld()->SpawnActor<CWzPart>(GetOwnerActor());
		PartsManager.SetPart(static_cast<EWzPartType>(i), Part);
	}
}

EWzPartType CWzPart::GetPartTypeByName(const std::string& InPartName)
{
	if (InPartName == "body")			return EWzPartType::Body;
	if (InPartName == "arm")				return EWzPartType::Arm;
	if (InPartName == "hand")			return EWzPartType::Hand;
	if (InPartName == "armOverHair")		return EWzPartType::ArmOverHair;
	if (InPartName == "lHand")			return EWzPartType::LHand;
	if (InPartName == "rHand")			return EWzPartType::RHand;
	return EWzPartType::End;
}

void CWzCharacterAnimator::Update(float InDeltaTime)
{
	if (CurrentAnimation == nullptr)
		return;

	CurrentAnimation->PlayAnimation(InDeltaTime);
}


