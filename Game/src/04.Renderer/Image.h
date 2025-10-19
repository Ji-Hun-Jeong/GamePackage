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
	CImage(std::unique_ptr<Graphics::CShaderResourceView> InShaderResourceView, const Vector3& InImageScale
		, uint32_t InStartSlot = 0)
		: ShaderResourceView(std::move(InShaderResourceView))
		, ImageScale(InImageScale)
		, StartSlot(InStartSlot)
	{}
	~CImage() {}

public:
	void BindToPipeline(Graphics::CRenderContext& InContext);
	const Vector3& GetImageScale() const { return ImageScale; }

private:
	std::unique_ptr<Graphics::CShaderResourceView> ShaderResourceView;
	Vector3 ImageScale;

	uint32_t StartSlot;
};

