#include "pch.h"
#include "Material.h"
#include "Base/RenderContext.h"

namespace Graphics
{
	void CMaterial::BindToPipeline(CRenderContext& InContext)
	{
		if (ShaderResourceViews.size())
			InContext.PSSetShaderResources(0, uint32_t(ShaderResourceViews.size()), ShaderResourceViews);
	}
}
