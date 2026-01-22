#pragma once
#include "01.Base/Actor/Component/Component.h"
#include "01.Base/Actor/StaticActor.h"
#include "WzPartData.h"

class CWzPart : public CStaticActor
{
	GENERATE_OBJECT(CWzPart)
public:
	CWzPart() : PartType(EWzPartType::End) {}
	~CWzPart() = default;

public:
	void Composite();
	void SetPartType(EWzPartType InPartType) { PartType = InPartType; }
	EWzPartType GetPartType() const { return PartType; }

	void SetPartData(const TWzPartData& InPartData) { PartData = InPartData; }
	const TWzPartData& GetPartData() const { return PartData; }

	static EWzPartType GetPartTypeByName(const std::string_view InPartName);

	void AttachChildPart(CWzPart& InChildPart)
	{
		InChildPart.OwnerPart = this;
		ChildParts.push_back(&InChildPart);
	}

private:
	EWzPartType PartType;

	TWzPartData PartData;

	CWzPart* OwnerPart = nullptr;
	std::vector<CWzPart*> ChildParts;

	Vector2 FinalNeck;
	Vector2 FinalNavel;
	Vector2 FinalHand;
	Vector2 FinalHandMove;

};

class CWzFrameData
{
public:
	CWzFrameData() = default;
	~CWzFrameData() = default;

public:
	bool ParseBasicFrameFromWz(const rapidjson::Value& InValue)
	{
		if (InValue.HasMember("png"))
		{
			const auto& PngValue = InValue["png"];
			if (PngValue.IsArray())
			{
				const auto& PngArray = PngValue.GetArray();
				for (const auto& Png : PngArray)
				{
					if (ParsePngValue(Png) == false)
						return false;
				}
			}
			else
				return ParsePngValue(PngValue);
		}
		else if (InValue.HasMember("uol"))
		{
			const auto& UolValue = InValue["uol"];
			if (UolValue.IsArray())
			{
				const auto& UolArray = UolValue.GetArray();
				for (const auto& Uol : UolArray)
				{
					if (ParseUolValue(Uol) == false)
						return false;
				}
			}
			else
				return ParseUolValue(UolValue);
		}

		if (InValue.HasMember("int16"))
		{
			const auto& Int16Value = InValue["int16"];
			if (Int16Value.IsObject() == false)
			{
				std::cout << "Int16Value Is Not Object\n";
				return false;
			}
			const auto& Int16Object = Int16Value.GetObject();
			const std::string_view Name = Int16Object["@name"].GetString();
			const std::string Value = Int16Object["@value"].GetString();

			Face = std::stoi(Value);
		}
		if (InValue.HasMember("int32"))
		{
			const auto& Int32Value = InValue["int32"];
			if (Int32Value.IsObject() == false)
			{
				std::cout << "Int32Value Is Not Object\n";
				return false;
			}
			const auto& Int32Object = Int32Value.GetObject();
			const std::string_view Name = Int32Object["@name"].GetString();
			const std::string Value = Int32Object["@value"].GetString();

			Delay = std::stoi(Value);
		}
		return true;
	}
	TWzPartData& GetPartDataRef(EWzPartType InPartType)
	{
		return PartDatas[static_cast<size_t>(InPartType)];
	}
	const TWzPartData& GetPartData(EWzPartType InPartType) const
	{
		return PartDatas[static_cast<size_t>(InPartType)];
	}

	void SetAction(const std::string& InAction) { Action = InAction; }
	void SetFace(int16_t InFace) { Face = InFace; }
	void SetDelay(int32_t InDelay) { Delay = InDelay; }
	void SetFrame(int32_t InFrame) { Frame = InFrame; }
	void SetMove(const Vector2& InMove) { Move = InMove; }

	int16_t GetFace() const { return Face; }
	int32_t GetDelay() const { return Delay; }
	int32_t GetFrame() const { return Frame; }
	Vector2 GetMove() const { return Move; }

private:
	bool ParsePngValue(const rapidjson::Value& InPngValue)
	{
		if (InPngValue.IsObject())
		{
			const auto& PngObject = InPngValue.GetObject();

			if (PngObject.HasMember("@name") == false)
				return false;
			const std::string_view Name = PngObject["@name"].GetString();
			EWzPartType WzPartType = CWzPart::GetPartTypeByName(Name);
			TWzPartData& PartData = GetPartDataRef(WzPartType);
			ParsePartPngObject(PngObject, &PartData);
		}
		return true;
	}
	bool ParseUolValue(const rapidjson::Value& InUolValue)
	{
		if (InUolValue.IsObject())
		{
			const auto& UolObject = InUolValue.GetObject();
			if (UolObject.HasMember("@name") == false)
				return false;
			const std::string_view Name = UolObject["@name"].GetString();
			EWzPartType WzPartType = CWzPart::GetPartTypeByName(Name);
			// PartData쪽으로 넘기기
		}
		return true;
	}

private:
	TWzPartData PartDatas[static_cast<size_t>(EWzPartType::End)];

	std::string Action;
	int16_t Face = 0;
	int32_t Delay = 0;
	int32_t Frame = 0;
	Vector2 Move;

};



class CWzCharacterAnimation
{
public:
	CWzCharacterAnimation() = default;
	CWzCharacterAnimation(const std::string& InAnimName)
		: AnimName(InAnimName)
	{
	}
	~CWzCharacterAnimation() = default;

public:
	bool ParseBasicAnimationFromWz(const rapidjson::Value& InValue)
	{
		if (InValue.HasMember("@name") == false)
			return false;
		const std::string_view Name = InValue["@name"].GetString();

		if (InValue.HasMember("dir") == false)
			return false;

		if (InValue["dir"].IsObject())
		{
			const auto& FirstFrameObject = InValue["dir"].GetObject();
			CWzFrameData FrameData;
			if (FrameData.ParseBasicFrameFromWz(FirstFrameObject) == false)
				return false;
			AddFrameData(FrameData);
		}
		else if (InValue["dir"].IsArray())
		{
			const auto& FrameArray = InValue["dir"].GetArray();
			for (const auto& FrameDataValue : FrameArray)
			{
				CWzFrameData FrameData;
				if (FrameData.ParseBasicFrameFromWz(FrameDataValue.GetObject()) == false)
					return false;
				AddFrameData(FrameData);
			}
		}
		else
			return false;

		AnimName = Name;

		return true;
	}

	void PlayAnimation(float InDeltaTime)
	{
		bFrameChange = false;
		if (bStop)
			return;

		ProgressTime += InDeltaTime * 0.5f;

		const CWzFrameData& CurrentFrame = Frames[CurrentFrameIndex];

		if (CurrentFrame.GetDelay() < ProgressTime * 1000.0f)
			EnterFrame(CurrentFrameIndex + 1);
	}

	void EnterFrame(size_t InFrameIndex)
	{
		bFrameChange = true;
		size_t FinalFrame = InFrameIndex;
		if (FinalFrame == Frames.size())
		{
			if (bLoop)
				FinalFrame = 0;
			else
			{
				bStop = true;
				return;
			}
		}

		CurrentFrameIndex = FinalFrame;
		ProgressTime = 0.0f;
		bStop = false;
	}

	const CWzFrameData& GetCurrentFrame() const { return Frames[CurrentFrameIndex]; }

	void AddFrameData(const CWzFrameData& InFrameData)
	{
		Frames.push_back(InFrameData);
	}

	bool IsStopped() const { return bStop; }
	bool IsFrameChanged() const { return bFrameChange; }

	void SetLoop(bool InLoop) { bLoop = InLoop; }

	const std::string& GetAnimName() const { return AnimName; }

private:
	std::string AnimName;
	std::vector<CWzFrameData> Frames;

	size_t CurrentFrameIndex = 0;

	float ProgressTime = 0.0f;

	bool bLoop = false;
	bool bStop = false;

	bool bFrameChange = false;

};