#include "pch.h"
#include "WzPart.h"

#include "GameCore.h"

//void CWzPart::Composite()
//{
//	// [공통 계산] 이미지 중심 대비 포인트들의 상대 오프셋 (Y축 반전 포함)
//	const std::wstring& ImagePath = SpriteRenderComponent->GetImagePath();
//	if (ImagePath.empty()) return;
//
//	Vector2 ImageCenter = GetImageScale(ImagePath) * 0.5f;
//	Vector2 Origin = PartData.Origin;
//
//	// 각 포인트들이 이미지 중심에서 얼마나 떨어져 있는지 계산
//	FinalNavel = Vector2(1.0f, -1.0f) * ((Origin + PartData.Map.Navel) - ImageCenter);
//	FinalHand = Vector2(1.0f, -1.0f) * ((Origin + PartData.Map.Hand) - ImageCenter);
//	FinalHandMove = Vector2(1.0f, -1.0f) * ((Origin + PartData.Map.HandMove) - ImageCenter);
//
//	EWzPartType PartType = GetPartType();
//	Vector2 ResultPos;
//
//	switch (PartType)
//	{
//	case EWzPartType::Body:
//		// 바디는 루트이므로 원점에 배치 (혹은 캐릭터 월드 좌표)
//		ResultPos;
//		break;
//
//	case EWzPartType::Arm:
//		// Arm(navel) -> Body(navel) 연결
//		ResultPos = OwnerPart->FinalNavel - FinalNavel;
//		// 다음 자식(LHand)을 위해 자신의 Hand 포인트 위치를 업데이트
//		// (현재 좌표에 자신의 Hand 오프셋을 더함)
//		FinalHand = ResultPos + FinalHand;
//		break;
//
//	case EWzPartType::LHand:
//		// LHand(handMove) -> Arm(hand) 연결
//		// Arm이 위에서 업데이트한 FinalHand(월드기준)를 사용함
//		//ResultPos = OwnerPart->FinalHand - FinalHandMove;
//		// Todo: 알아내긴 해야함
//		ResultPos = (Vector2(1.0f, -1.0f) * (Origin + PartData.Map.HandMove - ImageCenter));
//		break;
//
//	case EWzPartType::RHand:
//	{
//		// RHand(navel) -> Body(navel) 연결
//		// Todo: rHand계산법도 뭔가 이상함. 그냥 가만히 있던데 alert에서 rHand는
//		Vector3 WP = CWzUtils::GetWorldPositionFromOrigin(*SpriteRenderComponent, Origin);
//		ResultPos = Vector2(WP.x, WP.y);
//		//FinalNavel = ResultPos;
//		break;
//	}
//	case EWzPartType::Hand: // 보통의 Hand 파츠
//		ResultPos = OwnerPart->FinalNavel - FinalNavel;
//		break;
//
//	default:
//		break;
//	}
//
//	// 최종 트랜스폼 적용
//	Transform->SetPosition(Vector3(ResultPos.x, ResultPos.y, 0.0f));
//
//	// 자식들에게 전파
//	for (CWzPart* ChildPart : ChildParts)
//		ChildPart->Composite();
//}

void CWzPartsManager::InitalizeComponent()
{
	for (size_t i = 0; i < Parts.size(); ++i)
		Parts[i] = GetWorld()->SpawnActor<CWzPart>(GetOwnerActor());

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

void CWzPartsManager::CompositeParts(const TWzCharacterFrameData& InFrameData)
{
	const auto& PartDatas = InFrameData.PartDatas;
	for (const TWzPartData& PartData : PartDatas)
	{
		CWzPart* Part = GetPart(PartData.WzPartType);
		Part->GetSpriteRenderComponent()->SetDiffuseImage(PartData.OutLink);
		// 여기서 파츠를 세팅
	}
}