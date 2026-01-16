#include "pch.h"
#include "WzCharacterAnimator.h"

#include "GameCore.h"
#include "WzCharacterLoader.h"



void CWzPart::Composite()
{
	// 여기서 자신을 먼저 업데이트함. 부모를 참조해서
	const std::wstring& ImagePath = SpriteRenderComponent->GetImagePath();
	if (ImagePath.empty())
		return;
	Vector2 ImageCenter = GetImageScale(ImagePath) * 0.5f;
	Vector2 Origin = PartData.Origin;
	Vector2 Navel = PartData.Map.Navel;
	Vector2 Hand = PartData.Map.Hand;

	FinalNavel = Vector2(1.0f, -1.0f) * ((Origin + Navel) - ImageCenter);
	FinalHand = Vector2(1.0f, -1.0f) * ((Origin + Hand) - ImageCenter);
	if (OwnerPart)
	{
		FinalNavel = OwnerPart->FinalNavel - FinalNavel;
		FinalHand = OwnerPart->FinalNavel - FinalHand;
		EWzPartType PartType = GetPartType();
		switch (PartType)
		{
		case EWzPartType::Body:
			break;
		case EWzPartType::Arm:
			Transform->SetPosition(Vector3(FinalNavel.x, FinalNavel.y, 0.0f));
			break;
		case EWzPartType::Hand:
			Transform->SetPosition(Vector3(FinalNavel.x, FinalNavel.y, 0.0f));
			break;
		case EWzPartType::ArmOverHair:
			break;
		case EWzPartType::LHand:
			break;
		case EWzPartType::RHand:
			break;
		case EWzPartType::End:
			break;
		default:
			break;
		}
	}
	for (CWzPart* ChildPart : ChildParts)
		ChildPart->Composite();
}

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
	ArmPart->AttachChildPart(*LHandPart);
	ArmPart->AttachChildPart(*RHandPart);
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

	CurrentAnimation->PlayAnimation(InDeltaTime);

	if (CurrentAnimation->IsFrameChanged())
	{
		const auto& CurrentFrameData = CurrentAnimation->GetCurrentFrame();
		PartsManager->CompositeParts(CurrentFrameData);
	}
	PartsManager->UpdateParts();
}

