#pragma once
#include <nlohmann/json.hpp>
#include "04.Renderer/Image.h"
using CSerializer = nlohmann::json;

struct TFrame
{
	Vector3 Offset = Vector3(0.0f);
	std::wstring ImagePath;
	std::function<void()> FrameAction = nullptr;
	float Duration = 0.0f;
};

enum class EFrameIncreaseState
{
	Increase,
	Decrease
};
class CAnimation
{
public:
	CAnimation(bool bInInfinite = false)
		: CurrentFrameIndex(-1)
		, NextFrameIndex(0)
		, CurrentFrame(nullptr)
		, ProgressTime(0.0f)
		, bInfinite(bInInfinite)
	{
	}
	~CAnimation() = default;

public:
	void Serialize(CSerializer& InSerializer) const
	{
		CSerializer FrameArray = CSerializer::array();
		for (size_t i = 0; i < Frames.size(); ++i)
		{
			auto& Frame = Frames[i];
			CSerializer FrameData;
			FrameData["Order"] = uint32_t(i);
			FrameData["Offset"] = { Frame.Offset.x , Frame.Offset.y , Frame.Offset.z };
			FrameData["ImagePath"] = Frame.ImagePath;
			FrameArray.push_back(FrameData);
		}
		InSerializer.push_back(FrameArray);
	}
	void AddFrame(const TFrame& InFrame)
	{
		Frames.push_back(InFrame);
	}
	const TFrame& GetCurrentFrame() const { return *CurrentFrame; }
	void UpdateAnimationState(float InDeltaTime)
	{
		ProgressTime += InDeltaTime;
		if (CurrentFrame->Duration <= ProgressTime)
		{
			// 지금 프레임 증가 상태에 따라 끝에 도달했는지 체크
			// 루프가 아니면 멈추기
			// 루프면 프레임 증가 상태에 따라 다음 프레임으로 이동
			if (IsAnimationEnd())
			{
				if (bInfinite)
				{
					if (bPingPong)
						ChangeFrameIncreaseState();
					else
						RequestFrame(0);
				}
			}
			else
			{
				AdvanceFrame();
			}
		}
	}

	bool IsAnimationEnd() const
	{
		bool bEnd = false;
		if (bInfinite && bPingPong)
		{
			switch (FrameIncreaseState)
			{
			case EFrameIncreaseState::Increase:
				if (Frames.size() - 1 == CurrentFrameIndex)
					bEnd = true;
				break;
			case EFrameIncreaseState::Decrease:
				if (CurrentFrameIndex == 0)
					bEnd = true;
				break;
			default:
				break;
			}
		}
		else
		{
			if (Frames.size() - 1 == CurrentFrameIndex)
				bEnd = true;
		}
		return bEnd;
	}
	void AdvanceFrame()
	{
		if (bInfinite && bPingPong)
		{
			switch (FrameIncreaseState)
			{
			case EFrameIncreaseState::Increase:
				RequestFrame(CurrentFrameIndex + 1);
				break;
			case EFrameIncreaseState::Decrease:
				RequestFrame(CurrentFrameIndex - 1);
				break;
			default:
				break;
			}
			return;
		}
		else
			RequestFrame(CurrentFrameIndex + 1);
	}
	void ChangeFrameIncreaseState()
	{
		switch (FrameIncreaseState)
		{
		case EFrameIncreaseState::Increase:
			FrameIncreaseState = EFrameIncreaseState::Decrease;
			break;
		case EFrameIncreaseState::Decrease:
			FrameIncreaseState = EFrameIncreaseState::Increase;
			break;
		default:
			break;
		}
	}

	bool TryChangeFrame()
	{
		if (NextFrameIndex != CurrentFrameIndex)
		{
			CurrentFrameIndex = NextFrameIndex;
			CurrentFrame = &Frames[CurrentFrameIndex];
			return true;
		}
		return false;
	}

	void RequestFrame(size_t InFrameIndex)
	{
		NextFrameIndex = InFrameIndex;
		ProgressTime = 0.0f;
	}

	void UnifyFrameDuration(float InDuration)
	{
		for (auto& Frame : Frames)
			Frame.Duration = InDuration;
	}

	const TFrame& GetFrame(size_t InOrder) { return Frames[InOrder]; }

	void SetInfinite(bool bInInfinite) { bInfinite = bInInfinite; }
	void SetPingPong(bool bInPingPong)
	{
		if (bInPingPong)
		{
			bInfinite = true;
			FrameIncreaseState = EFrameIncreaseState::Increase;
		}
		else
		{
			bInfinite = false;
			FrameIncreaseState = EFrameIncreaseState::Increase;
		}
		bPingPong = bInPingPong;
	}

private:
	std::vector<TFrame> Frames;
	TFrame* CurrentFrame;

	size_t CurrentFrameIndex;
	size_t NextFrameIndex;

	float ProgressTime;

	bool bInfinite;
	bool bPingPong = false;
	EFrameIncreaseState FrameIncreaseState = EFrameIncreaseState::Increase;

};

