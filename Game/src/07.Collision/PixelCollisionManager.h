#pragma once
#include <Renderer/Base/RenderDevice.h>
#include <Renderer/Base/RenderContext.h>
#include "CollisionCommon.h"
#include "04.Renderer/RenderResourceLoader.h"

class CPixelCollider
{
	friend class CPixelCollisionManager;
public:
	CPixelCollider()
		: bCollision(false)
	{}
	~CPixelCollider()
	{}

public:
	void SetPosition(const Vector2& InPosition) { Position = InPosition; }
	void SetScale(const Vector2& InScale) { Scale = InScale; }
	bool IsCollision() const { return bCollision; }

private:
	Vector2 Position;
	Vector2 Scale;

	uint32_t bCollision;

};

class CPixelCollisionManager
{
public:
	CPixelCollisionManager(Graphics::CRenderDevice& InDevice, CRenderResourceLoader& InRenderResourceLoader, size_t InNumOfMaxCollider)
		: NumOfMaxCollider(InNumOfMaxCollider)
	{
		uint32_t ColliderSize = sizeof(CPixelCollider);
		ColliderBuffer = InRenderResourceLoader.CreateUAVBuffer(ColliderSize, NumOfMaxCollider);
		ColliderUAV = InRenderResourceLoader.CreateUAV(*ColliderBuffer.get(), NumOfMaxCollider);
		ColliderStagingBuffer = InRenderResourceLoader.CreateStagingBuffer(ColliderSize, NumOfMaxCollider);

		PixelColliders.reserve(NumOfMaxCollider);

		CollisionComputeShader = InDevice.CreateComputeShader(L"resources/shader/PixelCollisionComputeShader.hlsl");
	}
	~CPixelCollisionManager() = default;

public:
	void F()
	{
		// Context.CSSetShaderResource();
	}
	void RequestCollision(CPixelCollider& InPixelCollider)
	{
		PixelColliders.push_back(&InPixelCollider);
	}
	void ProgressCollisionCheck(Graphics::CRenderContext& InContext)
	{
		std::vector<CPixelCollider> ColliderCpuDatas;
		ColliderCpuDatas.reserve(PixelColliders.size());
		for (auto& PixelCollider : PixelColliders)
			ColliderCpuDatas.push_back(*PixelCollider);
		
		if (ColliderCpuDatas.size())
		{
			InContext.UpLoadBuffer(*ColliderStagingBuffer.get(), ColliderCpuDatas.data(), sizeof(CPixelCollider) * ColliderCpuDatas.size(), Graphics::EMapType::MAP_WRITE);
			InContext.CopyResource(*ColliderBuffer.get(), *ColliderStagingBuffer.get());

			InContext.CSSetShader(CollisionComputeShader.get());
			InContext.CSSetUnorderedAccessView(0, ColliderUAV.get());
			InContext.Dispatch(64, 1, 1);

			InContext.CopyResource(*ColliderStagingBuffer.get(), *ColliderBuffer.get());
			InContext.DownLoadBuffer(ColliderCpuDatas.data(), sizeof(CPixelCollider) * ColliderCpuDatas.size(), *ColliderStagingBuffer.get(), Graphics::EMapType::MAP_READ);

			for (size_t i = 0; i < ColliderCpuDatas.size(); ++i)
			{
				PixelColliders[i]->bCollision = ColliderCpuDatas[i].bCollision;
			}

			PixelColliders.clear();
		}
	}
	

private:
	std::vector<CPixelCollider*> PixelColliders;
	size_t NumOfMaxCollider;

	std::unique_ptr<Graphics::CBuffer> ColliderBuffer;
	std::unique_ptr<Graphics::CBuffer> ColliderStagingBuffer;
	std::unique_ptr<Graphics::CUnorderedAccessView> ColliderUAV;

	std::unique_ptr<Graphics::CComputeShader> CollisionComputeShader;

};

