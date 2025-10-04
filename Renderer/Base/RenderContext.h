#pragma once
#include "Interface/BlendState.h"
#include "Interface/ComputeShader.h"
#include "Interface/ConstBuffer.h"
#include "Interface/DepthStencilState.h"
#include "Interface/Draw.h"
#include "Interface/Mesh.h"
#include "Interface/PixelShader.h"
#include "Interface/RasterizerState.h"
#include "Interface/RenderTargetView.h"
#include "Interface/Sampler.h"
#include "Interface/ShaderResource.h"
#include "Interface/Topology.h"
#include "Interface/UnorderedAccessView.h"
#include "Interface/VertexShaderInputLayout.h"
#include "Interface/ViewPort.h"

namespace Graphics
{
	class RenderContext
	{
	public:
		RenderContext() = default;
		virtual ~RenderContext() = 0 {}

	public:
		void IASetPrimitiveTopology(ITopology& InTopology)
		{
			InTopology.IASetPrimitiveTopology();
		}
		void VSSetShader(IVertexShader& InVertexShader)
		{
			InVertexShader.IASetInputLayout();
			InVertexShader.VSSetShader();
		}
		void RSSetViewPort(IViewPort& InViewPort)
		{
			InViewPort.RSSetViewPort();
		}
		void RSSetRasterizerState(IRasterizerState& InRasterizerState)
		{
			InRasterizerState.RSSetState();
		}
		void PSSetShader(IPixelShader& InPixelShader)
		{
			InPixelShader.PSSetShader();
		}
		

	private:

	};
}


