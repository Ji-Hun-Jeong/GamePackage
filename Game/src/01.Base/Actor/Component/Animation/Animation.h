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
	CAnimation() = default;
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
	TFrame& AddFrame()
	{
		Frames.emplace_back(TFrame{});
		return Frames.back();
	}
	void SetLoop(bool bInLoop) { bLoop = bInLoop; }
	const TFrame& GetCurrentFrame() const { return Frames[CurrentFrameIndex]; }
	void UpdateAnimationState(float InDeltaTime)
	{
		bFrameChange = false;
		if (bFinish)
			return;

		ProgressTime += InDeltaTime;

		const TFrame& CurrentFrame = GetCurrentFrame();
		if (CurrentFrame.Duration <= ProgressTime)
			EnterFrame(CurrentFrameIndex + 1);
	}
	void EnterFrame(size_t InFrameIndex)
	{
		bFrameChange = true;

		size_t FinalFrameIndex = InFrameIndex;
		if (FinalFrameIndex == Frames.size())
		{
			if (bLoop)
				FinalFrameIndex = 0;
			else
				bFinish = true;
		}

		CurrentFrameIndex = FinalFrameIndex;
		ProgressTime = 0.0f;
	}
	bool IsFrameChanged() const { return bFrameChange; }
	bool IsFinish() const { return bFinish; }
	void UnifyFrameDuration(float InDuration)
	{
		for (auto& Frame : Frames)
			Frame.Duration = InDuration;
	}

private:
	std::vector<TFrame> Frames;

	size_t CurrentFrameIndex = 0;
	float ProgressTime = 0.0f;

	bool bLoop = false;
	bool bFrameChange = false;
	bool bFinish = false;

};

