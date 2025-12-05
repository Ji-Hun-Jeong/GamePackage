#pragma once
#include "RHI.h"
#include "Texture2D.h"

namespace Graphics
{
	enum EResourceDimension
	{
		RESOURCE_DIMENSION_UNKNOWN = 0,
		RESOURCE_DIMENSION_BUFFER = 1,
		RESOURCE_DIMENSION_TEXTURE1D = 2,
		RESOURCE_DIMENSION_TEXTURE2D = 3,
		RESOURCE_DIMENSION_TEXTURE3D = 4
	};

	typedef D3D_SRV_DIMENSION D3D11_SRV_DIMENSION;
	struct TShaderResourceViewDesc
	{
		EGIFormat Format;
		EResourceDimension ViewDimension;
		union
		{
			D3D11_BUFFER_SRV Buffer;
			D3D11_TEX1D_SRV Texture1D;
			D3D11_TEX1D_ARRAY_SRV Texture1DArray;
			D3D11_TEX2D_SRV Texture2D;
			D3D11_TEX2D_ARRAY_SRV Texture2DArray;
			D3D11_TEX2DMS_SRV Texture2DMS;
			D3D11_TEX2DMS_ARRAY_SRV Texture2DMSArray;
			D3D11_TEX3D_SRV Texture3D;
			D3D11_TEXCUBE_SRV TextureCube;
			D3D11_TEXCUBE_ARRAY_SRV TextureCubeArray;
			D3D11_BUFFEREX_SRV BufferEx;
		};
	};
	class CShaderResourceView : public CRHI
	{
	public:
		CShaderResourceView(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource)
			: CRHI(InResourceHandle, InEventReleaseResource)
		{}
		~CShaderResourceView() {}

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


