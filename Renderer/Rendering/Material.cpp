#include "pch.h"
#include "Material.h"
#include "Base/RenderContext.h"

namespace Graphics
{
	void CMaterial::BindToPipeline(CRenderContext& InContext)
	{
		InContext.PSSetShader(*PixelShader);
	}
}
