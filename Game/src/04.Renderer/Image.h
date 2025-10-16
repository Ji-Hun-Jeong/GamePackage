#pragma once
#include <Renderer/RHI/ShaderResourceView.h>
#include <Renderer/RHI/Buffer.h>

namespace Graphics
{
	class CRenderContext;
}

class CImage
{
public:
	CImage(std::unique_ptr<Graphics::CShaderResourceView> InShaderResourceView, std::unique_ptr<Graphics::CBuffer> InMeshResizeConstBuffer
	, uint32_t InStartSlot = 0)
		: ShaderResourceView(std::move(InShaderResourceView))
		, MeshResizeConstBuffer(std::move(InMeshResizeConstBuffer))
		, StartSlot(InStartSlot)
	{}
	~CImage() {}

public:
	void BindToPipeline(Graphics::CRenderContext& InContext);

private:
	std::unique_ptr<Graphics::CShaderResourceView> ShaderResourceView;
	std::unique_ptr<Graphics::CBuffer> MeshResizeConstBuffer;

	uint32_t StartSlot;
};

