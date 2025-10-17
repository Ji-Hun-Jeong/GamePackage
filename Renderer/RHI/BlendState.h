#pragma once
#include "RHI.h"

namespace Graphics
{
    enum class EBlend
    {
        BLEND_ZERO = 1,
        BLEND_ONE = 2,
        BLEND_SRC_COLOR = 3,
        BLEND_INV_SRC_COLOR = 4,
        BLEND_SRC_ALPHA = 5,
        BLEND_INV_SRC_ALPHA = 6,
        BLEND_DEST_ALPHA = 7,
        BLEND_INV_DEST_ALPHA = 8,
        BLEND_DEST_COLOR = 9,
        BLEND_INV_DEST_COLOR = 10,
        BLEND_SRC_ALPHA_SAT = 11,
        BLEND_BLEND_FACTOR = 14,
        BLEND_INV_BLEND_FACTOR = 15,
        BLEND_SRC1_COLOR = 16,
        BLEND_INV_SRC1_COLOR = 17,
        BLEND_SRC1_ALPHA = 18,
        BLEND_INV_SRC1_ALPHA = 19
    };

    enum class EBlendOP
    {
        BLEND_OP_ADD = 1,
        BLEND_OP_SUBTRACT = 2,
        BLEND_OP_REV_SUBTRACT = 3,
        BLEND_OP_MIN = 4,
        BLEND_OP_MAX = 5
    };

    enum class EColorWhiteEnable
    {
       COLOR_WRITE_ENABLE_RED = 1,
       COLOR_WRITE_ENABLE_GREEN = 2,
       COLOR_WRITE_ENABLE_BLUE = 4,
       COLOR_WRITE_ENABLE_ALPHA = 8,
       COLOR_WRITE_ENABLE_ALL = (((COLOR_WRITE_ENABLE_RED | COLOR_WRITE_ENABLE_GREEN) | COLOR_WRITE_ENABLE_BLUE) | COLOR_WRITE_ENABLE_ALPHA)
    };

    struct TRenderTargetBlendDesc
    {
        uint32_t BlendEnable;
        EBlend SrcBlend;
        EBlend DestBlend;
        EBlendOP BlendOp;
        EBlend SrcBlendAlpha;
        EBlend DestBlendAlpha;
        EBlendOP BlendOpAlpha;
        EColorWhiteEnable RenderTargetWriteMask;
    };

    struct TBlendDesc
    {
        uint32_t AlphaToCoverageEnable = 0;
        uint32_t IndependentBlendEnable = 0;
        TRenderTargetBlendDesc RenderTarget[8];
    };

	class CBlendState : public CRHI
	{
	public:
		CBlendState(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource)
			: CRHI(InResourceHandle, InEventReleaseResource)
		{}
		~CBlendState() = default;

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

	};
}


