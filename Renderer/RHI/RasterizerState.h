#pragma once
#include "RHI.h"

namespace Graphics
{
	enum class EFillMode : uint32_t
	{
		FillWireFrame = 2,
		FillSolid = 3
	};

	enum class ECullMode : uint32_t
	{
		CullNone = 1,
		CullFront = 2,
		CullBack = 3
	};

	struct TRasterizerDesc
	{
		EFillMode FillMode = EFillMode::FillSolid;
		ECullMode CullMode = ECullMode::CullNone;
		int FrontCounterClockwise = false;
		int DepthBias = 0;
		float DepthBiasClamp = 0.0f;
		float SlopeScaledDepthBias = 0;
		int DepthClipEnable = 0;
		int ScissorEnable = 0;
		int MultisampleEnable = 0;
		int AntialiasedLineEnable = 0;
	};

	class CRasterizerState : public CRHI
	{
	public:
		CRasterizerState(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource
			, const TRasterizerDesc& InRasterizerDesc)
			: CRHI(InResourceHandle, InEventReleaseResource)
			, RasterizerDesc(InRasterizerDesc)
		{}
		~CRasterizerState() = default;

	public:
		uint32_t GetRHIType() override
		{
			return StaticType();
		}
		static uint32_t StaticType()
		{
			static uint32_t RHIType = sRHIType++;
			return RHIType;
		}

	private:
		TRasterizerDesc RasterizerDesc;
	};
}

