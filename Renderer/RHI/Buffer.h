#pragma once
#include "RHI.h"

namespace Graphics
{
	enum class EBindFlags : uint32_t
	{
		InValid = 0,
		BindVertexBuffer = 0x1L,
		BindIndexBuffer = 0x2L,
		BindConstantBuffer = 0x4L,
		BindShaderResource = 0x8L,
		BindStreamOutput = 0x10L,
		BindRenderTarget = 0x20L,
		BindDepthStencil = 0x40L,
		BindUnorderedAccess = 0x80L,
		BindDecoder = 0x200L,
		BindVideoEncoder = 0x400L
	};

	enum class ECPUAccessFlags : uint32_t
	{
		CpuAccessImpossible = 0,
		CpuAccessWrite = 0x10000L,
		CpuAccessRead = 0x20000L,
	};

	struct TBufferDesc
	{
		uint32_t ByteWidth = 0;
		EUsage Usage = EUsage::UsageDefault;
		EBindFlags BindFlags = EBindFlags::InValid;
		ECPUAccessFlags CPUAccessFlags = ECPUAccessFlags::CpuAccessImpossible;
		uint32_t MiscFlags = 0;
		uint32_t StructureByteStride = 0;
	};

	struct TBufferInitalizeData
	{
		const void* CopyStartPoint = nullptr;
		uint32_t SysMemPitch = 0;
		uint32_t SysMemSlicePitch = 0;
	};

	enum class EMapType
	{
		MAP_READ = 1,
		MAP_WRITE = 2,
		MAP_READ_WRITE = 3,
		MAP_WRITE_DISCARD = 4,
		MAP_WRITE_NO_OVERWRITE = 5
	};

	// Usage가 D3D11_USAGE_IMMUTABLE인 경우에는 pInitialData가 필수. nullptr 넣으면 에러
	/*- DEFAULT: 일반적인 정점 버퍼, 인덱스 버퍼, 텍스처 등 GPU 중심 리소스
	  - IMMUTABLE : 절대 바뀌지 않는 텍스처, 셰이더 상수 등
	  - DYNAMIC : 프레임마다 바뀌는 상수 버퍼, UI 요소
	  - STAGING : GPU에서 읽은 데이터를 CPU로 가져올 때(예 : 스크린샷)*/

	enum class ETopology
	{
		PrimitiveTopologyUNDEFINED = 0,
		PrimitiveTopologyPOINTLIST = 1,
		PrimitiveTopologyLINELIST = 2,
		PrimitiveTopologyLINESTRIP = 3,
		PrimitiveTopologyTRIANGLELIST = 4,
		PrimitiveTopologyTRIANGLESTRIP = 5,
		PrimitiveTopologyLINELIST_ADJ = 10,
		PrimitiveTopologyLINESTRIP_ADJ = 11,
		PrimitiveTopologyTRIANGLELIST_ADJ = 12,
		PrimitiveTopologyTRIANGLESTRIP_ADJ = 13,
	};

	class CBuffer : public CRHI
	{
	public:
		CBuffer(size_t InResourceHandle, std::function<void(size_t)> InEventReleaseResource
			, const TBufferDesc& InBufferDesc, const TBufferInitalizeData* InBufferInitalizeData)
			: CRHI(InResourceHandle, InEventReleaseResource)
			, BufferDesc(InBufferDesc)
			, BufferInitalizeData(InBufferInitalizeData)
		{}
		~CBuffer() = default;

	public:
		const TBufferDesc& GetBufferDesc() const { return BufferDesc; }
		const TBufferInitalizeData* GetBufferInitalizeData() const { return BufferInitalizeData; }
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
		TBufferDesc BufferDesc;
		const TBufferInitalizeData* BufferInitalizeData;

	};
}
