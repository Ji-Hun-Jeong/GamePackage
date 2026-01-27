#include "pch.h"
#include "WzPart.h"

#include "GameCore.h"

void CWzPartsManager::InitalizeComponent()
{
	for (size_t i = 0; i < Parts.size(); ++i)
		Parts[i] = GetWorld()->SpawnActor<CWzPart>(GetOwnerActor());
	Head = GetWorld()->SpawnActor<CWzPart>(GetOwnerActor());
	Ear = GetWorld()->SpawnActor<CWzPart>(GetOwnerActor());
}

void CWzPartsManager::CompositeParts(const TWzCharacterFrameData& InFrameData, const TWzSkinFrameData& InSkinData)
{
	const TWzCharacterFrameData* FinalFrameData = &InFrameData;
	FinalFrameData = Wz::GetFinalFrameData(*FinalFrameData);

	const auto& PartDatas = FinalFrameData->PartDatas;
	for (size_t i = 0; i < static_cast<size_t>(EWzPartType::End); ++i)
	{
		EWzPartType PartType = static_cast<EWzPartType>(i);
		CWzPart* Part = Parts[i];
		auto Iter = PartDatas.find(PartType);
		if (Iter == PartDatas.end())
		{
			Part->Activate(false);
			Part->GetSpriteRenderComponent()->SetDiffuseImage(L"");
		}
		else
		{
			const TWzPartData* PartData = &Iter->second;
			PartData = Wz::GetFinalPartData(*PartData);
			Part->Activate(true);
			Part->GetSpriteRenderComponent()->SetDiffuseImage(PartData->OutLink);

			bool bBodyOrNotNavel = false;
			if ((PartType == EWzPartType::Body) || (PartData->Map.Navel == Vector2(0.0f)))
				bBodyOrNotNavel = true;

			Vector3 Offset;
			if (bBodyOrNotNavel)
			{
				GetOwnerActor()->Attach(Part);
				Offset = CWzUtils::GetWorldPositionFromWzPosition(*Part->GetSpriteRenderComponent()
					, PartData->Origin);
			}
			else
			{
				GetPart(EWzPartType::Body)->Attach(Part);

				const TWzPartData* BodyPartData = &PartDatas.find(EWzPartType::Body)->second;
				BodyPartData = Wz::GetFinalPartData(*BodyPartData);

				Offset = CWzUtils::GetWorldPositionFromWzPosition(*Part->GetSpriteRenderComponent()
					, PartData->Origin + PartData->Map.Navel) -
					CWzUtils::GetWorldPositionFromWzPosition(BodyPartData->OutLink
						, BodyPartData->Origin + BodyPartData->Map.Navel);
			}
			Part->GetTransform()->AddPositionOffset(Offset);
		}
	}

	const TWzPartData* BodyPartData = &PartDatas.find(EWzPartType::Body)->second;
	BodyPartData = Wz::GetFinalPartData(*BodyPartData);
	const Vector3 BodyNeckWorldPosition = CWzUtils::GetWorldPositionFromWzPosition(BodyPartData->OutLink,
		BodyPartData->Origin + BodyPartData->Map.Neck);

	TWzSkinPng* HeadSkinData = InSkinData.RefSkins[static_cast<uint8_t>(EWzSkinType::Head)];
	Head->GetSpriteRenderComponent()->SetDiffuseImage(HeadSkinData->OutLink);
	Vector3 Offset = CWzUtils::GetWorldPositionFromWzPosition(*Head->GetSpriteRenderComponent()
		, HeadSkinData->Origin + HeadSkinData->Map.Neck) - BodyNeckWorldPosition;
	Head->GetTransform()->AddPositionOffset(Offset);
	GetPart(EWzPartType::Body)->Attach(Head);

	TWzSkinPng* EarSkinData = InSkinData.RefSkins[static_cast<uint8_t>(EarType)];
	if (EarSkinData)
	{
		Ear->GetSpriteRenderComponent()->SetDiffuseImage(EarSkinData->OutLink);
		Offset = CWzUtils::GetWorldPositionFromWzPosition(*Ear->GetSpriteRenderComponent()
			, EarSkinData->Origin + EarSkinData->Map.Neck) - BodyNeckWorldPosition;
		Ear->GetTransform()->AddPositionOffset(Offset);
		GetPart(EWzPartType::Body)->Attach(Ear);
	}
	else
	{
		Ear->GetSpriteRenderComponent()->SetDiffuseImage(L"");
		Ear->Activate(false);
	}
}

namespace Wz
{
	TWzHumanAnimation TransformToHumanAnimation(const TWzAnimation& InCharacterAnimation, const TWzSkinAnimation& InSkinAnimation)
	{
		TWzHumanAnimation HumanAnimation;
		assert(InCharacterAnimation.Frames.size() == InSkinAnimation.Frames.size());

		for (size_t i = 0; i < InCharacterAnimation.Frames.size(); ++i)
			HumanAnimation.Frames.emplace_back(&InCharacterAnimation.Frames[i], &InSkinAnimation.Frames[i]
			, InCharacterAnimation.Frames[i].Delay);

		return HumanAnimation;
	}
}
