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
	class RenderDevice
	{
	public:
		RenderDevice() = default;
		virtual ~RenderDevice() = 0 {}

	public:
		virtual IRenderTargetView* GenerateMainRenderTargetView() = 0;

		virtual IViewPort* GenerateMainViewPort() = 0;

		virtual IMesh* GenerateMesh(void* InVertexData, size_t InVertexSize, size_t InNumOfVertex
			, void* InIndexData, size_t InIndexSize, size_t InNumOfIndex) = 0;

		virtual IConstBuffer* GenerateConstBuffer(const std::vector<CpuConstData>& InCpuData) = 0;

		virtual IVertexShader* GenerateVertexShaderAndInputLayout(const std::string& InPath
			, const std::vector<InputElementDesc>& InInputElements) = 0;

		virtual IPixelShader* GeneratePixelShader(const std::string& InPath) = 0;

		virtual ISampler* GenerateSampler() = 0;

		virtual IRasterizerState* GenerateSolidCWState() = 0;

		virtual IDepthStencilState* GenerateBasicDepthStencilState() = 0;

		virtual ITopology* GenerateTopology(eTopology InTopology) = 0;

		virtual IPresenter* GeneratePresenter() = 0;

		virtual IShaderResource* GenerateShaderResource(const std::vector<std::string>& InPaths) = 0;

		virtual IBlendState* GenerateBlendState(const float* InBlendFactor) = 0;

		virtual IComputeShader* GenerateComputeShader(const std::string& InPath) = 0;

		// virtual IUnorderedAccessView* GenerateUnorderedAccessView(const std::vector<BufferDesc>& _BufferDesc) =0;

	};
}