#pragma once
#include "01.Base/Actor/StaticActor.h"

class CFader : public CStaticActor
{
	GENERATE_OBJECT(CFader)
public:
	CFader()
		: HoldingTime(1.0f)
		, ProgressTime(0.0f)
		, FaderState(EFaderState::None)
	{
		RenderComponent->SetPSO(EPSOType::Color);
		RenderComponent->SetPixelConstBufferData(0, sizeof(TColorData));
		RenderComponent->UpdatePixelConstBufferData(0, &ColorData, sizeof(TColorData));
	}
	~CFader() = default;

public:
	void FadeIn(float InHoldingTime)
	{
		HoldingTime = InHoldingTime;
		FaderState = EFaderState::FadeIn;
	}
	void FadeOut(float InHoldingTime)
	{
		HoldingTime = InHoldingTime;
		FaderState = EFaderState::FadeOut;
	}
	void Update(float InDeltaTime) override
	{
		CStaticActor::Update(InDeltaTime);

		switch (FaderState)
		{
		case EFaderState::FadeIn:
			OnFadeIn(InDeltaTime);
			break;
		case EFaderState::FadeOut:
			OnFadeOut(InDeltaTime);
			break;
		case EFaderState::None:
			return;
			break;
		}

		RenderComponent->UpdatePixelConstBufferData(0, &ColorData, sizeof(TColorData));
	}

private:
	enum class EFaderState
	{
		FadeIn,
		FadeOut,
		None,
	};
	void SetState(EFaderState InFaderState)
	{
		FaderState = InFaderState;
	}
	void OnFadeIn(float InDeltaTime)
	{
		// 밝아지는(투명해져야함)
		ColorData.Transparency = 1.0f - ProgressTime / HoldingTime;
		ProgressTime += InDeltaTime;
		if (HoldingTime <= ProgressTime)
		{
			ColorData.Transparency = 0.0f;
			SetState(EFaderState::None);
		}
	}
	void OnFadeOut(float InDeltaTime)
	{
		// 어두워지는
		ColorData.Transparency = ProgressTime / HoldingTime;
		ProgressTime += InDeltaTime;
		if (HoldingTime <= ProgressTime)
		{
			ColorData.Transparency = 1.0f;
			SetState(EFaderState::None);
		}
	}

private:
	float HoldingTime;
	float ProgressTime;
	struct TColorData
	{
		Vector3 Color{ 0.0f,0.0f,0.0f };
		float Transparency = 0.0f;
	} ColorData;

	EFaderState FaderState;
};

