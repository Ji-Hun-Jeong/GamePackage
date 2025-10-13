#pragma once
#include "RHI/PixelShader.h"

namespace Graphics
{
	struct TMaterialData
	{
		std::wstring PixelShaderPath;
	};

	class CMaterial
	{
	public:
		CMaterial(std::unique_ptr<CPixelShader> InPixelShader)
			: PixelShader(std::move(InPixelShader))
		{}
		~CMaterial() = default;

	public:
		void SetPixelShader(std::unique_ptr<CPixelShader> InPixelShader)
		{
			PixelShader.reset();
			PixelShader = std::move(InPixelShader);
		}
		void BindToPipeline(class CRenderContext& InContext);

	private:
		std::unique_ptr<CPixelShader> PixelShader;

	};
}

