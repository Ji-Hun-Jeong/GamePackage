#pragma once
#include "01.Base/Actor/Component/Component.h"
#include "01.Base/Actor/StaticActor.h"

enum class EWzPartType
{
	Body,
	Arm,
	Hand,
	ArmOverHair,
	LHand,
	RHand,
	End,
};

struct TWzPartData
{
	std::string Value;
	Vector2 Origin;
	struct TMap
	{
		Vector2 Neck;
		Vector2 Navel;
		Vector2 Hand;
		Vector2 HandMove;
	} Map;
	std::string Z;
	std::string Group;
	std::string OutLink;
};

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

	static EWzPartType GetPartTypeByName(const std::string& InPartName);

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
	TWzPartData PartDatas[static_cast<size_t>(EWzPartType::End)];

	std::string Action;
	int16_t Face = 0;
	int32_t Delay = 0;
	int32_t Frame = 0;
	Vector2 Move;

};

class CWzPartsManager
{
public:
	explicit CWzPartsManager(std::array<CWzPart*, static_cast<size_t>(EWzPartType::End)>&& InParts);
	~CWzPartsManager() = default;

public:
	// 얘가 직접 만들어줘야할듯?
	void UpdateParts();
	void CompositeParts(const CWzFrameData& InFrameData);
	CWzPart* GetPart(EWzPartType InPartType) const
	{
		if (InPartType == EWzPartType::End)
		{
			std::cout << "[CWzCharacterAnimator::GetPart] 잘못된 파트 타입 요청: End" << std::endl;
			return nullptr;
		}
		return Parts[static_cast<size_t>(InPartType)];
	}

private:
	std::array<CWzPart*, static_cast<size_t>(EWzPartType::End)> Parts;

};

class CWzCharacterAnimation
{
	friend class CWzCharacterAnimator;
public:
	CWzCharacterAnimation(const std::string& InAnimName)
		: AnimName(InAnimName)
	{
	}
	~CWzCharacterAnimation() = default;

public:
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

private:
	std::string AnimName;
	std::vector<CWzFrameData> Frames;

	size_t CurrentFrameIndex = 0;

	float ProgressTime = 0.0f;

	bool bLoop = false;
	bool bStop = false;

	bool bFrameChange = false;

};

class CWzCharacterAnimator : public CComponent
{
	GENERATE_OBJECT(CWzCharacterAnimator)
public:
	CWzCharacterAnimator() = default;
	~CWzCharacterAnimator() = default;

public:
	void InitalizeComponent() override;
	void Update(float InDeltaTime);
	void SetCurrentAnimation(const std::string& InAnimName, bool bInLoop = false)
	{
		auto Iter = WzCharacterAnimations.find(InAnimName);
		if (Iter == WzCharacterAnimations.end())
		{
			std::cout << "[CWzCharacterAnimator::PlayAnimation] 애니메이션을 찾을 수 없습니다: " << InAnimName << std::endl;
			return;
		}
		CWzCharacterAnimation& Animation = Iter->second;
		CurrentAnimation = &Animation;

		CurrentAnimation->SetLoop(bInLoop);
		CurrentAnimation->EnterFrame(0);
	}
	CWzCharacterAnimation& GetAnimationRef(const std::string& InAnimName)
	{
		if (WzCharacterAnimations.contains(InAnimName) == false)
			WzCharacterAnimations.emplace(InAnimName, InAnimName);
		return WzCharacterAnimations.at(InAnimName);
	}
	CWzCharacterAnimation* GetAnimation(const std::string& InAnimName)
	{
		auto Iter = WzCharacterAnimations.find(InAnimName);
		if (Iter == WzCharacterAnimations.end())
			return nullptr;
		return &Iter->second;
	}

private:
	std::unique_ptr<CWzPartsManager> PartsManager;

	std::unordered_map<std::string, CWzCharacterAnimation> WzCharacterAnimations;

	CWzCharacterAnimation* CurrentAnimation = nullptr;

};

