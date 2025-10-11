#include "pch.h"
#include "Game.h"

CGame::CGame(UINT InScreenWidth, UINT InScreenHeight)
	: Core::CApplication(InScreenWidth, InScreenHeight)
	, World()
	, GraphicInfra(Window.GetWindowHandle())
	, Device(GraphicInfra.GetDevice())
	, Context(GraphicInfra.GetContext())
	, SwapChain(GraphicInfra.GetSwapChain())
{
	struct Vertex
	{
		Vector3 Position;
		Vector3 Color;
	};

	std::vector<Vertex> Vertices =
	{
		{Vector3(-1.0f, -1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f)},
		{Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f)},
		{Vector3(1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)}
	};

	Graphics::TBufferDesc VertexBufferDesc;
	VertexBufferDesc.BindFlags = Graphics::EBindFlags::BindVertexBuffer;
	VertexBufferDesc.ByteWidth = uint32_t(sizeof(Vertex) * Vertices.size());
	VertexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
	VertexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;

	Graphics::TBufferInitalizeData VertexBufferInitData;
	VertexBufferInitData.CopyStartPoint = Vertices.data();
	auto VertexBuffer = Device.CreateBuffer(VertexBufferDesc, &VertexBufferInitData);

	std::vector<uint32_t> Indices =
	{
		0, 1, 2
	};
	Graphics::TBufferDesc IndexBufferDesc;
	IndexBufferDesc.BindFlags = Graphics::EBindFlags::BindIndexBuffer;
	IndexBufferDesc.ByteWidth = uint32_t(sizeof(uint32_t) * Indices.size());
	IndexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
	IndexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;

	Graphics::TBufferInitalizeData IndexBufferInitData;
	IndexBufferInitData.CopyStartPoint = Indices.data();
	auto IndexBuffer = Device.CreateBuffer(IndexBufferDesc, &IndexBufferInitData);

	Mesh = new Graphics::CMesh(*VertexBuffer.get(), *IndexBuffer.get(), Graphics::EGIFormat::GI_FORMAT_R32_UINT, uint32_t(Indices.size())
		, uint32_t(sizeof(Vertex)), 0);

	std::vector<Graphics::TInputElementDesc> InputElementDescs =
	{
		{Graphics::ESementicName::Position, Graphics::EFormat::Vector3, 0, Graphics::EInputClass::VertexData},
		{Graphics::ESementicName::Color, Graphics::EFormat::Vector3, 12, Graphics::EInputClass::VertexData}
	};
	auto VertexShaderAndInputLayout = Device.CreateVertexShaderAndInputLayout(L"resources/shader/BasicVertexShader.hlsl", InputElementDescs);
	auto& VertexShader = VertexShaderAndInputLayout.first;
	auto& InputLayout = VertexShaderAndInputLayout.second;

	Graphics::TRasterizerDesc RasterizerDesc;
	RasterizerDesc.FillMode = Graphics::EFillMode::FillSolid;
	RasterizerDesc.CullMode = Graphics::ECullMode::CullBack;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.MultisampleEnable = true;
	auto RasterizerState = Device.CreateRasterizerState(RasterizerDesc);

	auto PixelShader = Device.CreatePixelShader(L"resources/shader/BasicPixelShader.hlsl");

	const Graphics::CTexture2D* WindowTextureBuffer = SwapChain.GetWindowTextureBuffer();
	auto RenderTargetView = Device.CreateRenderTargetView(*WindowTextureBuffer);


	Graphics::TViewPort ViewPort;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.Width = static_cast<float>(1280);
	ViewPort.Height = static_cast<float>(960);
	ViewPort.MinDepth = 0.0f;
	ViewPort.MaxDepth = 1.0f;

	Context.RSSetViewPort(ViewPort);
	Context.OMSetRenderTarget(1, *RenderTargetView.get(), nullptr);
	Context.IASetPrimitiveTopology(Graphics::ETopology::PrimitiveTopologyTRIANGLELIST);
	Context.IASetInputLayout(*InputLayout.get());

	Context.VSSetShader(*VertexShader.get());

	Context.RSSetState(*RasterizerState.get());

	Context.PSSetShader(*PixelShader.get());
	Mesh->BindToPipeline(Context);

}

CGame::~CGame()
{
}

bool CGame::Process()
{
	World.Update();
	Context.DrawIndexed(Mesh->GetIndexCount());
	SwapChain.Present();
	return true;
}

void CGame::ShutDown()
{
}