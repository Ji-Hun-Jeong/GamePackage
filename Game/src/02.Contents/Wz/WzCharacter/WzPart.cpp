#include "pch.h"
#include "WzPart.h"

#include "GameCore.h"

void CWzPartsManager::InitalizeComponent()
{
	for (size_t i = 0; i < Parts.size(); ++i)
		Parts[i] = GetWorld()->SpawnActor<CWzPart>(GetOwnerActor());
}

void CWzPartsManager::CompositeParts(const TWzCharacterFrameData& InFrameData)
{
	const auto& PartDatas = InFrameData.PartDatas;
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
			const TWzPartData& PartData = Iter->second;
			Part->Activate(true);
			Part->GetSpriteRenderComponent()->SetDiffuseImage(PartData.OutLink);

			bool bBodyOrNotNavel = false;
			if ((PartType == EWzPartType::Body) || (PartData.Map.Navel == Vector2(0.0f)))
				bBodyOrNotNavel = true;

			Vector3 Offset;
			if (bBodyOrNotNavel)
			{
				GetOwnerActor()->Attach(Part);
				Offset = CWzUtils::GetWorldPositionFromWzPosition(*Part->GetSpriteRenderComponent()
					, PartData.Origin);
			}
			else
			{
				GetPart(EWzPartType::Body)->Attach(Part);

				const auto& BodyPartData = PartDatas.find(EWzPartType::Body)->second;
				Offset = CWzUtils::GetWorldPositionFromWzPosition(*Part->GetSpriteRenderComponent()
					, PartData.Origin + PartData.Map.Navel) -
					CWzUtils::GetWorldPositionFromWzPosition(BodyPartData.OutLink
						, BodyPartData.Origin + BodyPartData.Map.Navel);
			}
			Part->GetTransform()->AddPositionOffset(Offset);
		}
	}
}