#pragma once
#include "RHI/PixelShader.h"

namespace Graphics
{
	// 텍스쳐, PixelShaderConstantBuffer, SamplerState <- 이건 PSO..?
	struct TMaterialData
	{
		
	};

	class CMaterial
	{
	public:
		CMaterial()
		{}
		~CMaterial() = default;

	public:
		void BindToPipeline(class CRenderContext& InContext);

	private:


	};
}

