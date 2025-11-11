#pragma once
#include <Renderer/Base/RenderDevice.h>
#include <Renderer/Base/RenderContext.h>
#include "CollisionCommon.h"
#include "04.Renderer/RenderResourceLoader.h"

class CPixelCollisionObject
{
	friend class CPixelCollisionManager;
public:
	CPixelCollisionObject()
		: Collisions{false}
	{}
	~CPixelCollisionObject()
	{}

public:
	void SetPosition(const Vector2& InPosition) { Position = InPosition; }
	void SetScale(const Vector2& InScale) { Scale = InScale; }

private:
	void ClearState() 
	{
		for (auto& Collision : Collisions)
			Collision = false;
	}
	void Collision(size_t InSlot, bool bInCollision)
	{
		Collisions[InSlot] = bInCollision;
	}
	void CopyCollisionResult(const CPixelCollisionObject& InOther)
	{
		for (size_t i = 0; i < 6; ++i)
		{
			Collisions[i] = InOther.Collisions[i];
		}
	}

private:
	Vector2 Position;
	Vector2 Scale;

	uint32_t Collisions[6] = { 0 };

};


enum class EPixelMapType
{
	FloorMap,
	LadderMap,
	RopeMap,
	WallMap,
	MonsterWallMap,
	OtherMap,
};

struct TPixelCollisionConstBufferData
{
	uint32_t ScreenWidth;
	uint32_t ScreenHeight;
	uint32_t NumOfColliders;
	uint32_t Dummy;
};

class CPixelCollisionManager
{
public:
	CPixelCollisionManager(Graphics::CRenderDevice& InDevice, CRenderResourceLoader& InRenderResourceLoader, size_t InNumOfMaxCollider)
		: NumOfMaxCollider(InNumOfMaxCollider)
		, RenderResourceLoader(InRenderResourceLoader)
		, PixelMaps{nullptr}
	{
		uint32_t ColliderSize = sizeof(CPixelCollisionObject);
		ColliderBuffer = InRenderResourceLoader.CreateUAVBuffer(ColliderSize, NumOfMaxCollider);
		ColliderUAV = InRenderResourceLoader.CreateUAV(*ColliderBuffer.get(), NumOfMaxCollider);
		ColliderStagingBuffer = InRenderResourceLoader.CreateStagingBuffer(ColliderSize, NumOfMaxCollider);

		PixelCollisionObjects.reserve(NumOfMaxCollider);

		CollisionComputeShader = InDevice.CreateComputeShader(L"resources/shader/PixelCollisionComputeShader.hlsl");

		PixelCollisionConstBuffer = InRenderResourceLoader.CreateConstBuffer(sizeof(TPixelCollisionConstBufferData));

		Graphics::TSamplerDesc SamplerDesc;
		SamplerDesc.Filter = Graphics::EFilter::FILTER_MIN_MAG_MIP_POINT;
		SamplerDesc.AddressU = Graphics::ETextureAddressMode::TEXTURE_ADDRESS_CLAMP;
		SamplerDesc.AddressV = Graphics::ETextureAddressMode::TEXTURE_ADDRESS_CLAMP;
		SamplerDesc.AddressW = Graphics::ETextureAddressMode::TEXTURE_ADDRESS_CLAMP;
		SamplerDesc.ComparisonFunc = Graphics::EComparisonFunc::COMPARISON_NEVER;
		SamplerDesc.MinLOD = 0;
		SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		PointSampler = InDevice.CreateSamplerState(SamplerDesc);
	}
	~CPixelCollisionManager() = default;

public:
	void SetPixelMap(EPixelMapType InPixelMapType, const std::wstring& InPixelMapPath)
	{
		CImage* Image = RenderResourceLoader.LoadImageFromFile(InPixelMapPath);
		PixelMaps[size_t(InPixelMapType)] = &Image->GetSRV();
	}
	void RequestCollisionCheck(CPixelCollisionObject& InPixelCollisionObject)
	{
		InPixelCollisionObject.ClearState();
		PixelCollisionObjects.push_back(&InPixelCollisionObject);
	}
	void ProgressCollisionCheck(Graphics::CRenderContext& InContext, uint32_t InScreenWidth, uint32_t InScreenHeight)
	{
		if (PixelMaps.empty())
			return;

		std::vector<CPixelCollisionObject> ColliderCpuDatas;
		ColliderCpuDatas.reserve(PixelCollisionObjects.size());
		for (auto& PixelCollider : PixelCollisionObjects)
			ColliderCpuDatas.push_back(*PixelCollider);

		for (size_t i = 0; i < PixelMaps.size(); ++i)
			InContext.CSSetShaderResource(uint32_t(i), PixelMaps[i]);

		InContext.CSSetSampler(0, PointSampler.get());

		if (ColliderCpuDatas.size())
		{
			TPixelCollisionConstBufferData PixelCollisionConstBufferData;
			PixelCollisionConstBufferData.ScreenWidth = InScreenWidth;
			PixelCollisionConstBufferData.ScreenHeight = InScreenHeight;
			PixelCollisionConstBufferData.NumOfColliders = uint32_t(ColliderCpuDatas.size());

			InContext.UpLoadBuffer(*PixelCollisionConstBuffer.get(), &PixelCollisionConstBufferData, sizeof(PixelCollisionConstBufferData)
				, Graphics::EMapType::MAP_WRITE_DISCARD);
			InContext.CSSetConstantBuffer(0, PixelCollisionConstBuffer.get());

			InContext.UpLoadBuffer(*ColliderStagingBuffer.get(), ColliderCpuDatas.data(), sizeof(CPixelCollisionObject) * ColliderCpuDatas.size(), Graphics::EMapType::MAP_WRITE);
			InContext.CopyResource(*ColliderBuffer.get(), *ColliderStagingBuffer.get());

			InContext.CSSetShader(CollisionComputeShader.get());
			InContext.CSSetUnorderedAccessView(0, ColliderUAV.get());
			InContext.Dispatch(8, 1, 1);

			InContext.CopyResource(*ColliderStagingBuffer.get(), *ColliderBuffer.get());
			InContext.DownLoadBuffer(ColliderCpuDatas.data(), sizeof(CPixelCollisionObject) * ColliderCpuDatas.size(), *ColliderStagingBuffer.get(), Graphics::EMapType::MAP_READ);

			for (size_t i = 0; i < ColliderCpuDatas.size(); ++i)
			{
				PixelCollisionObjects[i]->CopyCollisionResult(ColliderCpuDatas[i]);
				/*for (size_t j = 0; j < 6; ++j)
				{
					printf("%zd: %d\n", j, PixelCollisionObjects[i]->Collisions[j]);
				}*/
			}
			printf("%d\n", PixelCollisionObjects[0]->Collisions[1]);
			PixelCollisionObjects.clear();
		}

		for (auto& PixelMap : PixelMaps)
			PixelMap = nullptr;
	}
	

private:
	CRenderResourceLoader& RenderResourceLoader;

	std::vector<CPixelCollisionObject*> PixelCollisionObjects;
	size_t NumOfMaxCollider;
	std::array<const Graphics::CShaderResourceView*, 6> PixelMaps;

	std::unique_ptr<Graphics::CSamplerState> PointSampler;

	std::unique_ptr<Graphics::CBuffer> ColliderBuffer;
	std::unique_ptr<Graphics::CBuffer> ColliderStagingBuffer;
	std::unique_ptr<Graphics::CUnorderedAccessView> ColliderUAV;

	std::unique_ptr<Graphics::CComputeShader> CollisionComputeShader;

	std::unique_ptr<Graphics::CBuffer> PixelCollisionConstBuffer;
};

