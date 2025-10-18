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
	void SaveFrameData(CSerializer& InSerializer) const
	{
		CSerializer FrameArray = CSerializer::array();
		for (size_t i = 0; i < Frames.size(); ++i)
		{
			auto& Frame = Frames[i];
			CSerializer FrameData;
			FrameData["order"] = uint32_t(i);
			FrameData["offset"] = { {"x", Frame.Offset.x}, {"y", Frame.Offset.y} ,{"z", Frame.Offset.z}};
			FrameData["image_path"] = Frame.ImagePath;
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
			if (Frames.size() - 1 == CurrentFrameIndex)
			{
				if (bInfinite)
					RequestFrame(0);
			}
			else
				RequestFrame(CurrentFrameIndex + 1);
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

private:
	std::vector<TFrame> Frames;
	TFrame* CurrentFrame;

	size_t CurrentFrameIndex;
	size_t NextFrameIndex;

	float ProgressTime;

	bool bInfinite;

};

