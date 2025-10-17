#pragma once
#include "RHI.h"

namespace Graphics
{
    enum class EFilter
    {
        FILTER_MIN_MAG_MIP_POINT = 0,
        FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
        FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
        FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
        FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
        FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
        FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
        FILTER_MIN_MAG_MIP_LINEAR = 0x15,
        FILTER_ANISOTROPIC = 0x55,
        FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
        FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
        FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
        FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
        FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
        FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
        FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
        FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
        FILTER_COMPARISON_ANISOTROPIC = 0xd5,
        FILTER_MINIMUM_MIN_MAG_MIP_POINT = 0x100,
        FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x101,
        FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x104,
        FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x105,
        FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x110,
        FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x111,
        FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x114,
        FILTER_MINIMUM_MIN_MAG_MIP_LINEAR = 0x115,
        FILTER_MINIMUM_ANISOTROPIC = 0x155,
        FILTER_MAXIMUM_MIN_MAG_MIP_POINT = 0x180,
        FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x181,
        FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x184,
        FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x185,
        FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x190,
        FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x191,
        FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x194,
        FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR = 0x195,
        FILTER_MAXIMUM_ANISOTROPIC = 0x1d5
    };

    enum class ETextureAddressMode
    {
        TEXTURE_ADDRESS_WRAP = 1,
        TEXTURE_ADDRESS_MIRROR = 2,
        TEXTURE_ADDRESS_CLAMP = 3,
        TEXTURE_ADDRESS_BORDER = 4,
        TEXTURE_ADDRESS_MIRROR_ONCE = 5
    };

    enum class EComparisonFunc
    {
        COMPARISON_NEVER = 1,
        COMPARISON_LESS = 2,
        COMPARISON_EQUAL = 3,
        COMPARISON_LESS_EQUAL = 4,
        COMPARISON_GREATER = 5,
        COMPARISON_NOT_EQUAL = 6,
        COMPARISON_GREATER_EQUAL = 7,
        COMPARISON_ALWAYS = 8
    };

    struct TSamplerDesc
    {
        EFilter Filter = EFilter::FILTER_MIN_MAG_MIP_POINT;
        ETextureAddressMode AddressU = ETextureAddressMode::TEXTURE_ADDRESS_WRAP;
        ETextureAddressMode AddressV = ETextureAddressMode::TEXTURE_ADDRESS_WRAP;
        ETextureAddressMode AddressW = ETextureAddressMode::TEXTURE_ADDRESS_WRAP;
        float MipLODBias = 0.0f;
        uint32_t MaxAnisotropy = 0;
        EComparisonFunc ComparisonFunc = EComparisonFunc::COMPARISON_NEVER;
        float BorderColor[4] = { 0.0f };
        float MinLOD = 0.0f;
        float MaxLOD = 10.0f;
    };

	class CSamplerState : public CRHI
	{
	public:
		CSamplerState(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource)
            : CRHI(InResourceHandle, InEventReleaseResource)
        {}
		~CSamplerState() {}

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


