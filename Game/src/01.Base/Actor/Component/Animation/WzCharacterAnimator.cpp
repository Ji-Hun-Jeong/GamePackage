#include "pch.h"
#include "WzCharacterAnimator.h"

#include "GameCore.h"
#include "WzCharacterLoader.h"

CWzCharacterAnimator::CWzCharacterAnimator()
{
	for (size_t i = 0; i < static_cast<size_t>(EWzPartType::End); ++i)
		PartActors[i] = GetWorld()->SpawnActor<CWzPart>(GetOwnerActor());
}