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
	CImage(std::unique_ptr<Graphics::CShaderResourceView> InShaderResourceView, std::unique_ptr<Graphics::CTexture2D> InTexture2D
		, uint32_t InStartSlot = 0)
		: ShaderResourceView(std::move(InShaderResourceView))
		, Texture2D(std::move(InTexture2D))
	{}
	~CImage() {}

public:
	const Graphics::CShaderResourceView& GetSRV() const { return *ShaderResourceView.get(); }
	const Graphics::CTexture2D& GetTexture2D() const { return *Texture2D.get(); }

private:
	std::unique_ptr<Graphics::CShaderResourceView> ShaderResourceView;
	std::unique_ptr<Graphics::CTexture2D> Texture2D;

};

