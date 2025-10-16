#pragma once
#include "RHI/PixelShader.h"

namespace Graphics
{
	// 텍스쳐, PixelShaderConstantBuffer, SamplerState <- 이건 PSO..?
	using MaterialKey = uint32_t;
	struct TMaterialData
	{
		MaterialKey Key;
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

