#pragma once
#include "RHI/PixelShader.h"
#include "RHI/ShaderResourceView.h"

namespace Graphics
{
	// 텍스쳐, PixelShaderConstantBuffer, SamplerState <- 이건 PSO..?
	using MaterialKey = uint32_t;

	struct TMaterialData
	{
		std::wstring ImagePath;
		MaterialKey Key = 0;
	};

	class CMaterial
	{
	public:
		CMaterial()
		{}
		~CMaterial() = default;

	public:
		void AddShaderResourceView(std::unique_ptr<CShaderResourceView> InShaderResourceView)
		{
			ShaderResourceViews.push_back(std::move(InShaderResourceView));
		}

	private:
		std::vector<std::unique_ptr<CShaderResourceView>> ShaderResourceViews;

	};
}

