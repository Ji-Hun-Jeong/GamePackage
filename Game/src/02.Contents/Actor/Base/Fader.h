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
		RenderComponent->SetConstBuffer(EShaderType::PixelShader, 0, sizeof(TColorData));
		RenderComponent->UpdateConstBuffer(EShaderType::PixelShader, 0, &ColorData, sizeof(TColorData));
	}
	~CFader() = default;

public:
	void FadeIn(float InHoldingTime)
	{
		if (FaderState == EFaderState::None)
		{
			HoldingTime = InHoldingTime;
			FaderState = EFaderState::FadeIn;
		}
	}
	void FadeOut(float InHoldingTime)
	{
		if (FaderState == EFaderState::None)
		{
			HoldingTime = InHoldingTime;
			FaderState = EFaderState::FadeOut;
		}
	}
	void SetStateEndEvent(std::function<void()> InStateEndEvent)
	{
		if (FaderState == EFaderState::None)
			StateEndEvent = InStateEndEvent;
	}
	void Update(float InDeltaTime) override
	{
		CStaticActor::Update(InDeltaTime);

		Transform->SetScale(Vector3(float(CWindowManager::GetInst().GetScreenWidth()), float(CWindowManager::GetInst().GetScreenHeight()), Transform->GetScale().z));
		
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
	}
	void FinalUpdate() override
	{
		CStaticActor::FinalUpdate();
	}
	void CaptureSnapShot() override
	{
		CStaticActor::CaptureSnapShot();
		RenderComponent->UpdateConstBuffer(EShaderType::PixelShader, 0, &ColorData, sizeof(TColorData));
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
		if (FaderState == EFaderState::None)
		{
			ProgressTime = 0.0f;
			if (StateEndEvent)
				StateEndEvent();
		}
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
	std::function<void()> StateEndEvent;

};

