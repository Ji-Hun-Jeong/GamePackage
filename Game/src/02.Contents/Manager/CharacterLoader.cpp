#include "pch.h"
#include "CharacterLoader.h"

CCharacterLoader::CCharacterLoader()
{
}

/*
* {
  "dir": {
	"@name": "00002000.img",
	"dir": [
	  {
		"@name": "info",
		"string": [
		  { "@name": "islot", "@value": "Bd" },
		  { "@name": "vslot", "@value": "Bd" }
		],
		"int32": { "@name": "cash", "@value": "0" }
	  },
	  {
		"@name": "walk1",
		"dir": [
		  {
			"@name": "0",
			"png": [
			  {
				"@name": "body",
				"@value": "iVBOR... (생략)",
				"vector": { "@name": "origin", "@value": "19, 32" },
				"dir": {
				  "@name": "map",
				  "vector": [
					{ "@name": "neck", "@value": "-4, -32" },
					{ "@name": "navel", "@value": "-6, -20" }
				  ]
				},
				"string": [
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

void CCharacterLoader::InitalizeCharacter(CCharacter& OutCharacter)
{
	OutCharacter.AddPart("body");
	OutCharacter.AddPart("arm");
	OutCharacter.AddPart("hand");
	OutCharacter.AddPart("armOverHair");
	OutCharacter.AddPart("lHand");
	OutCharacter.AddPart("rHand");

	OutCharacter.GetPart("body")->AddComponent<CAnimator>();
	OutCharacter.GetPart("arm")->AddComponent<CAnimator>();
	OutCharacter.GetPart("hand")->AddComponent<CAnimator>();
	OutCharacter.GetPart("armOverHair")->AddComponent<CAnimator>();
	OutCharacter.GetPart("lHand")->AddComponent<CAnimator>();
	OutCharacter.GetPart("rHand")->AddComponent<CAnimator>();
}

void CCharacterLoader::CoverToCharacter(const std::string& InCharacterName, const std::string& InAnimName, CCharacter& OutCharacter)
{
	if (OutCharacter.GetPart("body") == nullptr || OutCharacter.GetPart("body")->GetAnimator() == nullptr)
		assert(0);
	if(OutCharacter.GetPart("arm") == nullptr || OutCharacter.GetPart("arm")->GetAnimator() == nullptr)
		assert(0);
	if(OutCharacter.GetPart("hand") == nullptr || OutCharacter.GetPart("hand")->GetAnimator() == nullptr)
		assert(0);
	if(OutCharacter.GetPart("armOverHair") == nullptr || OutCharacter.GetPart("armOverHair")->GetAnimator() == nullptr)
		assert(0);
	if(OutCharacter.GetPart("lHand") == nullptr || OutCharacter.GetPart("lHand")->GetAnimator() == nullptr)
		assert(0);
	if (OutCharacter.GetPart("rHand") == nullptr || OutCharacter.GetPart("rHand")->GetAnimator() == nullptr)
		assert(0);

	auto Iter = CharacterDatas.find(InCharacterName);
	if (Iter == CharacterDatas.end())
		return;
	CCharacterData& CharacterData = Iter->second;

	if (CharacterData.AnimDatas.contains(InAnimName) == false)
		return;

	const CAnimData& AnimData = CharacterData.AnimDatas.at(InAnimName);
	for (size_t FrameIndex = 0; FrameIndex < AnimData.KeyFrameDatas.size(); ++FrameIndex)
	{
		const TKeyFrameData& KeyFrameData = AnimData.KeyFrameDatas[FrameIndex];

		// 미리 모든 파트를 추가해둔다.
		// origin, neck, navel, hand 정보로는 파트의 애니메이션 프레임의 Offset을 계산한다.
		CompositeCharacterFrame(InAnimName, KeyFrameData, FrameIndex, OutCharacter);
	}
}

void CCharacterLoader::CompositeCharacterFrame(const std::string& InAnimName, const TKeyFrameData& InKeyFrameData, size_t InFrameNumber, CCharacter& OutCharacter)
{
	CPart* BodyPart = OutCharacter.GetPart("body");
	CPart* ArmPart = OutCharacter.GetPart("arm");
	CPart* HandPart = OutCharacter.GetPart("hand");
	CPart* ArmOverHairPart = OutCharacter.GetPart("armOverHair");
	CPart* LHandPart = OutCharacter.GetPart("lHand");
	CPart* RHandPart = OutCharacter.GetPart("rHand");

	CAnimation* Animation = BodyPart->GetAnimator()->GetAnimation(InAnimName);
	if (Animation == nullptr)
		BodyPart->GetAnimator()->AddAnimation(InAnimName, std::make_unique<CAnimation>());
	AddFrameToPart(InAnimName, InKeyFrameData, InKeyFrameData.Body, "body", InFrameNumber, OutCharacter);

	Animation = ArmPart->GetAnimator()->GetAnimation(InAnimName);
	if (Animation == nullptr)
		ArmPart->GetAnimator()->AddAnimation(InAnimName, std::make_unique<CAnimation>());
	AddFrameToPart(InAnimName, InKeyFrameData, InKeyFrameData.Arm, "arm", InFrameNumber, OutCharacter);

	Animation = HandPart->GetAnimator()->GetAnimation(InAnimName);
	if (Animation == nullptr)
		HandPart->GetAnimator()->AddAnimation(InAnimName, std::make_unique<CAnimation>());
	AddFrameToPart(InAnimName, InKeyFrameData, InKeyFrameData.Hand, "hand", InFrameNumber, OutCharacter);
}

void CCharacterLoader::AddFrameToPart(const std::string& InAnimName, const TKeyFrameData& InKeyFrameData, const TPartData& InPartData
	, const std::string& InPartName, size_t InFrameNumber, CCharacter& OutCharacter)
{
	CPart* Part = OutCharacter.GetPart(InPartName);
	CAnimation* OutAnimation = Part->GetAnimator()->GetAnimation(InAnimName);
	if (IsRefValue(InPartData.Value))
	{
		// Frame을 복사한다.
		std::string Path = DetachRelativePath(InPartData.Value);
		std::string FramePath = Path;
		uint32_t FrameNumber = GetFrameNumberInRefPath(FramePath);

		std::string RefAnimName;
		if (IsFrameRefPath(Path) == false)
			RefAnimName = GetAnimationNameInRefPath(Path, &FramePath);

		TFrame CopyFrame;
		if (RefAnimName.empty())
			CopyFrame = OutAnimation->GetFrame(FrameNumber);
		else
			CopyFrame = Part->GetAnimator()->GetAnimation(RefAnimName)->GetFrame(FrameNumber);

		OutAnimation->AddFrame(CopyFrame);
	}
	else
	{
		// Frame을 직접 만든다.
		TFrame Frame;
		Frame.ImagePath = std::wstring(InPartData.OutLink.begin(), InPartData.OutLink.end());
		Frame.Duration = GetDurationFromDelay(InKeyFrameData.Delay);

		Frame.Offset = CalculatePartOffset(InAnimName, InPartName, InFrameNumber, OutCharacter, InKeyFrameData, InPartData);
		OutAnimation->AddFrame(Frame);
	}
}

Vector3 CCharacterLoader::CalculatePartOffset(const std::string& InAnimName, const std::string& InPartName, size_t InFrameNumber
	, const CCharacter& InCharacter, const TKeyFrameData& InKeyFrameData, const TPartData& InPartData)
{
	if (IsValidPartData(InPartData) == false)
		return Vector3(0.0f);

	Vector2 ResultOffset;
	CPart* BodyPart = InCharacter.GetPart("body");

	Vector2 BodyOrigin = InKeyFrameData.Body.Origin;
	Vector2 BodyNavel = InKeyFrameData.Body.Map.Navel;
	std::wstring BodyImagePath = std::wstring(InKeyFrameData.Body.OutLink.begin(), InKeyFrameData.Body.OutLink.end());
	Vector2 BodyImageCenter = InCharacter.GetOwnerActor()->GetImageScale(BodyImagePath) * 0.5f;
	Vector2 BodyNavelOffset = Vector2(1.0f, -1.0f) * ((BodyOrigin + BodyNavel) - BodyImageCenter);


	if (InPartName == "arm")
	{
		Vector2 ArmOrigin = InKeyFrameData.Arm.Origin;
		Vector2 ArmNavel = InKeyFrameData.Arm.Map.Navel;
		Vector2 ArmHand = InKeyFrameData.Arm.Map.Hand;
		std::wstring ArmImagePath = std::wstring(InKeyFrameData.Arm.OutLink.begin(), InKeyFrameData.Arm.OutLink.end());
		Vector2 ArmImageCenter = InCharacter.GetOwnerActor()->GetImageScale(ArmImagePath) * 0.5f;
		Vector2 ArmNavelOffset = Vector2(1.0f, -1.0f) * ((ArmOrigin + ArmNavel) - ArmImageCenter);
		Vector2 ArmHandOffset = Vector2(1.0f, -1.0f) * (ArmOrigin + ArmHand - ArmImageCenter);

		ResultOffset = BodyNavelOffset - ArmNavelOffset;
	}
	else if (InPartName == "hand")
	{
		Vector2 ArmOrigin = InKeyFrameData.Arm.Origin;
		Vector2 ArmHand = InKeyFrameData.Arm.Map.Hand;
		std::wstring ArmImagePath = std::wstring(InKeyFrameData.Arm.OutLink.begin(), InKeyFrameData.Arm.OutLink.end());
		Vector2 ArmImageCenter = InCharacter.GetOwnerActor()->GetImageScale(ArmImagePath) * 0.5f;
		Vector2 ArmHandOffset = Vector2(1.0f, -1.0f) * (ArmOrigin + ArmHand - ArmImageCenter);

		Vector2 HandOrigin = InKeyFrameData.Hand.Origin;
		Vector2 HandNavel = InKeyFrameData.Hand.Map.Navel;
		std::wstring HandImagePath = std::wstring(InKeyFrameData.Hand.OutLink.begin(), InKeyFrameData.Hand.OutLink.end());
		Vector2 HandImageCenter = InCharacter.GetOwnerActor()->GetImageScale(HandImagePath) * 0.5f;

		Vector2 HandNavelOffset = Vector2(1.0f, -1.0f) * (HandOrigin + HandNavel - HandImageCenter);
		Vector2 HandPosition = BodyNavelOffset - ArmHandOffset - HandNavelOffset;
		ResultOffset = HandPosition;
	}
	return Vector3(ResultOffset.x, ResultOffset.y, 1.0f);
}

void CCharacterLoader::ParseNode(const std::string& InNodeName, const std::string& InValue)
{
	if (InNodeName == "body")
	{
		CurrentEditPartData = &CurrentEditKeyFrameData->Body;
		CurrentEditPartData->Value = InValue;
	}
	else if (InNodeName == "arm")
	{
		CurrentEditPartData = &CurrentEditKeyFrameData->Arm;
		CurrentEditPartData->Value = InValue;
	}
	else if (InNodeName == "hand")
	{
		Vector2 HandPos;
		if (StrToVec2(InValue, &HandPos))
			CurrentEditPartData->Map.Hand = HandPos;
		else
		{
			CurrentEditPartData = &CurrentEditKeyFrameData->Hand;
			CurrentEditPartData->Value = InValue;
		}
	}
	else if (InNodeName == "armOverHair")
	{
		CurrentEditPartData = &CurrentEditKeyFrameData->ArmOverHair;
		CurrentEditPartData->Value = InValue;
	}
	else if (InNodeName == "lHand")
	{
		CurrentEditPartData = &CurrentEditKeyFrameData->LHand;
		CurrentEditPartData->Value = InValue;
	}
	else if (InNodeName == "rHand")
	{
		CurrentEditPartData = &CurrentEditKeyFrameData->RHand;
		CurrentEditPartData->Value = InValue;
	}
	else if (InNodeName == "neck")
	{
		Vector2 NeckPos;
		if (StrToVec2(InValue, &NeckPos) && CurrentEditPartData)
			CurrentEditPartData->Map.Neck = NeckPos;
		else
			bEmpty = true;
	}
	else if (InNodeName == "navel")
	{
		Vector2 NavelPos;
		if (StrToVec2(InValue, &NavelPos) && CurrentEditPartData)
			CurrentEditPartData->Map.Navel = NavelPos;
		else
			bEmpty = true;
	}
	else if (InNodeName == "origin")
	{
		Vector2 OriginPos;
		if (StrToVec2(InValue, &OriginPos) && CurrentEditPartData)
			CurrentEditPartData->Origin = OriginPos;
		else
			bEmpty = true;
	}
	else if (InNodeName == "z")
	{
		if (CurrentEditPartData)
			CurrentEditPartData->Z = InValue;
		else
			bEmpty = true;
	}
	else if (InNodeName == "group")
	{
		if (CurrentEditPartData)
			CurrentEditPartData->Group = InValue;
		else
			bEmpty = true;
	}
	else if (InNodeName == "_outlink")
	{
		if (CurrentEditPartData)
			CurrentEditPartData->OutLink = "resources/image/" + InValue + ".png";
		else
			bEmpty = true;
	}
	else if (InNodeName == "face")
	{
		int16_t FaceValue = static_cast<int16_t>(std::stoi(InValue));
		CurrentEditKeyFrameData->Face = FaceValue;
	}
	else if (InNodeName == "delay")
	{
		int32_t FaceValue = static_cast<int32_t>(std::stoi(InValue));
		CurrentEditKeyFrameData->Delay = FaceValue * 4.0f;
	}
	else if (InNodeName == "frame")
	{
		int32_t FrameValue = static_cast<int32_t>(std::stoi(InValue));
		CurrentEditKeyFrameData->Frame = FrameValue;
	}
	else if (InNodeName == "move")
	{
		Vector2 Move;
		if (StrToVec2(InValue, &Move))
			CurrentEditKeyFrameData->Move = Move;
	}
	else if (InNodeName == "action")
	{
		CurrentEditKeyFrameData->Action = InValue;
	}
	else
		bEmpty = true;
}

