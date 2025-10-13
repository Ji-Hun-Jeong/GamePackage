#pragma once
#include "RHI/PixelShader.h"

namespace Graphics
{
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

