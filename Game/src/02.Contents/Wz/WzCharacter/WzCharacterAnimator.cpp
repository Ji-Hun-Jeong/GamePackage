#include "pch.h"
#include "WzCharacterAnimator.h"

#include "GameCore.h"

void CWzCharacterAnimator::InitalizeComponent()
{
	std::array<CWzPart*, static_cast<size_t>(EWzPartType::End)> Parts;
	for (size_t i = 0; i < Parts.size(); ++i)
	{
		Parts[i] = GetWorld()->SpawnActor<CWzPart>(GetOwnerActor());
		Parts[i]->SetPartType(EWzPartType(i));
	}
	PartsManager = std::make_unique<CWzPartsManager>(std::move(Parts));
}

void CWzCharacterAnimator::Update(float InDeltaTime)
{
	if (CurrentAnimation == nullptr)
		return;
	if (CurrentAnimation->IsFrameChanged())
	{
		const auto& CurrentFrameData = CurrentAnimation->GetCurrentFrame();
		PartsManager->CompositeParts(CurrentFrameData);
	}
	PartsManager->UpdateParts();

	CurrentAnimation->PlayAnimation(InDeltaTime);
}
