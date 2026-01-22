#include "pch.h"
#include "WzCharacterAnimation.h"

/*
{
  "dir":
  {
	"@name": "00002000.img",
	"dir":
	[
	  {
		"@name": "info",
		"string":
		[
		  { "@name": "islot", "@value": "Bd" },
		  { "@name": "vslot", "@value": "Bd" }
		],
		"int32": { "@name": "cash", "@value": "0" }
	  },
	  {
		"@name": "walk1",
		"dir":
		[
		  {
			"@name": "0",
			"png":
			[
			  {
				"@name": "body",
				"@value": "iVBOR... (생략)",
				"vector": { "@name": "origin", "@value": "19, 32" },
				"dir":
				{
				  "@name": "map",
				  "vector":
				  [
					{ "@name": "neck", "@value": "-4, -32" },
					{ "@name": "navel", "@value": "-6, -20" }
				  ]
				},
				"string":
				[
				  { "@name": "z", "@value": "body" },
				  { "@name": "group", "@value": "skin" },
				  { "@name": "_outlink", "@value": "Character/_Canvas/00002000.img/walk1/0/body" }
				]
			  },
			  {
				"@name": "arm",
				"@value": "iVBOR... (생략)",
				"vector": { "@name": "origin", "@value": "6, 8" },
				"dir": {
				  "@name": "map",
				  "vector": [
					{ "@name": "navel", "@value": "-12, 2" },
					{ "@name": "hand", "@value": "1, 5" }
				  ]
				},
				"string": [
				  { "@name": "z", "@value": "arm" },
				  { "@name": "group", "@value": "skin" },
				  { "@name": "_outlink", "@value": "Character/_Canvas/00002000.img/walk1/0/arm" }
				]
			  }
			],
			"int16": { "@name": "face", "@value": "1" },
			"int32": { "@name": "delay", "@value": "180" }
		  },
		  {
			"@name": "1",
			"png": [
			  {
				"@name": "body",
				"@value": "iVBOR... (생략)",
				"vector": { "@name": "origin", "@value": "16, 32" }
			  }
			],
			"int32": { "@name": "delay", "@value": "180" }
		  }
		]
	  }
	]
  }
}
*/

void CWzPart::Composite()
{
	// [공통 계산] 이미지 중심 대비 포인트들의 상대 오프셋 (Y축 반전 포함)
	const std::wstring& ImagePath = SpriteRenderComponent->GetImagePath();
	if (ImagePath.empty()) return;

	Vector2 ImageCenter = GetImageScale(ImagePath) * 0.5f;
	Vector2 Origin = PartData.Origin;

	// 각 포인트들이 이미지 중심에서 얼마나 떨어져 있는지 계산
	FinalNavel = Vector2(1.0f, -1.0f) * ((Origin + PartData.Map.Navel) - ImageCenter);
	FinalHand = Vector2(1.0f, -1.0f) * ((Origin + PartData.Map.Hand) - ImageCenter);
	FinalHandMove = Vector2(1.0f, -1.0f) * ((Origin + PartData.Map.HandMove) - ImageCenter);

	EWzPartType PartType = GetPartType();
	Vector2 ResultPos;

	switch (PartType)
	{
	case EWzPartType::Body:
		// 바디는 루트이므로 원점에 배치 (혹은 캐릭터 월드 좌표)
		ResultPos;
		break;

	case EWzPartType::Arm:
		// Arm(navel) -> Body(navel) 연결
		ResultPos = OwnerPart->FinalNavel - FinalNavel;
		// 다음 자식(LHand)을 위해 자신의 Hand 포인트 위치를 업데이트
		// (현재 좌표에 자신의 Hand 오프셋을 더함)
		FinalHand = ResultPos + FinalHand;
		break;

	case EWzPartType::LHand:
		// LHand(handMove) -> Arm(hand) 연결
		// Arm이 위에서 업데이트한 FinalHand(월드기준)를 사용함
		//ResultPos = OwnerPart->FinalHand - FinalHandMove;
		// Todo: 알아내긴 해야함
		ResultPos = (Vector2(1.0f, -1.0f) * (Origin + PartData.Map.HandMove - ImageCenter));
		break;

	case EWzPartType::RHand:
	{
		// RHand(navel) -> Body(navel) 연결
		// Todo: rHand계산법도 뭔가 이상함. 그냥 가만히 있던데 alert에서 rHand는
		Vector3 WP = CWzUtils::GetWorldPositionFromOrigin(*SpriteRenderComponent, Origin);
		ResultPos = Vector2(WP.x, WP.y);
		//FinalNavel = ResultPos;
		break;
	}
	case EWzPartType::Hand: // 보통의 Hand 파츠
		ResultPos = OwnerPart->FinalNavel - FinalNavel;
		break;

	default:
		break;
	}

	// 최종 트랜스폼 적용
	Transform->SetPosition(Vector3(ResultPos.x, ResultPos.y, 0.0f));

	// 자식들에게 전파
	for (CWzPart* ChildPart : ChildParts)
		ChildPart->Composite();
}

EWzPartType CWzPart::GetPartTypeByName(const std::string_view InPartName)
{
	if (InPartName == "body")			return EWzPartType::Body;
	if (InPartName == "arm")				return EWzPartType::Arm;
	if (InPartName == "hand")			return EWzPartType::Hand;
	if (InPartName == "armOverHair")		return EWzPartType::ArmOverHair;
	if (InPartName == "lHand")			return EWzPartType::LHand;
	if (InPartName == "rHand")			return EWzPartType::RHand;
	return EWzPartType::End;
}