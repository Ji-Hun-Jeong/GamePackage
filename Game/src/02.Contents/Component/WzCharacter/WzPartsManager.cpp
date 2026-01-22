#include "pch.h"
#include "WzPartsManager.h"

CWzPartsManager::CWzPartsManager(std::array<CWzPart*, static_cast<size_t>(EWzPartType::End)>&& InParts)
	: Parts(std::move(InParts))
{
	for (size_t i = 0; i < Parts.size(); ++i)
		assert(Parts[i]);

	CWzPart* BodyPart = GetPart(EWzPartType::Body);
	CWzPart* ArmPart = GetPart(EWzPartType::Arm);
	CWzPart* HandPart = GetPart(EWzPartType::Hand);
	CWzPart* ArmOverHairPart = GetPart(EWzPartType::ArmOverHair);
	CWzPart* LHandPart = GetPart(EWzPartType::LHand);
	CWzPart* RHandPart = GetPart(EWzPartType::RHand);

	BodyPart->AttachChildPart(*ArmPart);
	BodyPart->AttachChildPart(*HandPart);
	BodyPart->AttachChildPart(*RHandPart);
	BodyPart->AttachChildPart(*LHandPart);
}

void CWzPartsManager::UpdateParts()
{
	GetPart(EWzPartType::Body)->Composite();
}

void CWzPartsManager::CompositeParts(const CWzFrameData& InFrameData)
{
	for (size_t PartIndex = 0; PartIndex < static_cast<size_t>(EWzPartType::End); ++PartIndex)
	{
		EWzPartType PartType = static_cast<EWzPartType>(PartIndex);

		const TWzPartData& PartData = InFrameData.GetPartData(PartType);

		CWzPart* Part = GetPart(PartType);
		Part->SetPartData(PartData);
		Part->GetSpriteRenderComponent()->SetDiffuseImage(std::wstring(PartData.OutLink.begin(), PartData.OutLink.end()));
	}
}