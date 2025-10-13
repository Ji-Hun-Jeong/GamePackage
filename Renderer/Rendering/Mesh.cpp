#include "pch.h"
#include "Mesh.h"
#include "Base/RenderContext.h"

namespace Graphics
{
	void CMesh::BindToPipeline(CRenderContext& InContext)
	{
		InContext.IASetPrimitiveTopology(PrimitiveTopology);
		InContext.IASetVertexBuffer(*VertexBuffer, &Stride, &Offset);
		InContext.IASetIndexBuffer(*IndexBuffer, IndexFormat, 0);
		InContext.DrawIndexed(IndexCount);
	}
}

