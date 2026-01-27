#pragma once
#include "Common/Json.h"
#include "04.Renderer/Image.h"

struct TFrame
{
	Vector3 Offset = Vector3(0.0f);
	std::wstring ImagePath;
	float Duration = 0.0f;
};

struct TAnimation
{
	std::vector<TFrame> Frames;
	bool bLoop = false;
};

class CAnimation
{
public:
	CAnimation() = default;
	~CAnimation() = default;

public:
	void Serialize(rapidjson::Value& InSerializer) const
	{
		/*CSerializer FrameArray = CSerializer::array();
		for (size_t i = 0; i < Frames.size(); ++i)
		{
			auto& Frame = Frames[i];
			CSerializer FrameData;
			FrameData["Order"] = uint32_t(i);
			FrameData["Offset"] = { Frame.Offset.x , Frame.Offset.y , Frame.Offset.z };
			FrameData["ImagePath"] = Frame.ImagePath;
			FrameArray.push_back(FrameData);
		}
		InSerializer.push_back(FrameArray);*/
	}

	void DeSerialize(const rapidjson::Value& InValue)
	{
		if (!InValue.IsObject()) return;

		if (InValue.HasMember("Loop"))
			bLoop = InValue["Loop"].GetBool();

		if (InValue.HasMember("Frames") && InValue["Frames"].IsArray())
		{
			Frames.clear();

			const auto& FrameArray = InValue["Frames"];
			for (rapidjson::SizeType i = 0; i < FrameArray.Size(); ++i)
			{
				const auto& FrameData = FrameArray[i];
				TFrame& NewFrame = AddFrame();

				// 이미지 경로
				if (FrameData.HasMember("ImagePath"))
				{
					std::string IP = FrameData["ImagePath"].GetString();
					NewFrame.ImagePath = std::wstring(IP.begin(), IP.end());
				}

				// 오프셋 (배열 형태 [x, y, z] 가정)
				if (FrameData.HasMember("Offset") && FrameData["Offset"].IsArray())
				{
					NewFrame.Offset.x = FrameData["Offset"][0].GetFloat();
					NewFrame.Offset.y = FrameData["Offset"][1].GetFloat();
					NewFrame.Offset.z = FrameData["Offset"][2].GetFloat();
				}

				// 지속 시간 (딜레이)
				if (FrameData.HasMember("Duration"))
					NewFrame.Duration = FrameData["Duration"].GetFloat();
			}
		}
	}

	TFrame& AddFrame()
	{
		Frames.emplace_back(TFrame{});
		return Frames.back();
	}
	void SetLoop(bool bInLoop) { bLoop = bInLoop; }
	const TFrame& GetCurrentFrame() const { return Frames[CurrentFrameIndex]; }
	size_t GetCurrentFrameNumber() const { return CurrentFrameIndex; }
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
	void Reserve(size_t InCapacity)
	{
		Frames.reserve(InCapacity);
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
	void UnifyOffset(const Vector3& InOffset)
	{
		for (auto& Frame : Frames)
			Frame.Offset = InOffset;
	}

private:
	std::vector<TFrame> Frames;

	size_t CurrentFrameIndex = 0;
	float ProgressTime = 0.0f;

	bool bLoop = false;
	bool bFrameChange = false;
	bool bFinish = false;

};
