#include "pch.h"
#include "World.h"

class CCO : public CActor
{
public:
	void BeginPlay() override
	{
		CObject::BeginPlay();
		std::cout << "CCO\n";
	}
};

class CO : public CActor
{
	GenerateObject()
public:
	CO()
	{

	}
	void BeginPlay() override
	{
		CObject::BeginPlay();
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

		Graphics::TMeshData MeshData;
		MeshData.VertexBufferDesc.BindFlags = Graphics::EBindFlags::BindVertexBuffer;
		MeshData.VertexBufferDesc.ByteWidth = uint32_t(sizeof(Vertex) * Vertices.size());
		MeshData.VertexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
		MeshData.VertexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;
		
		MeshData.VertexBufferInitData.CopyStartPoint = Vertices.data();

		std::vector<uint32_t> Indices =
		{
			0, 1, 2
		};
		MeshData.IndexBufferDesc.BindFlags = Graphics::EBindFlags::BindIndexBuffer;
		MeshData.IndexBufferDesc.ByteWidth = uint32_t(sizeof(uint32_t) * Indices.size());
		MeshData.IndexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
		MeshData.IndexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;

		MeshData.IndexBufferInitData.CopyStartPoint = Indices.data();
		MeshData.IndexFormat = Graphics::EGIFormat::GI_FORMAT_R32_UINT;
		MeshData.IndexCount = uint32_t(Indices.size());
		MeshData.Stride = sizeof(Vertex);
		MeshData.Offset = 0;

		Graphics::TMaterialData MaterialData;

		RenderComponent = GetWorld()->NewObject<CRenderComponent>();
		RenderComponent->SetMesh(MeshData);
		RenderComponent->SetMaterial(MaterialData);
		RenderComponent->SetPSO(EPSOType::ColorBasic);

		Transform = GetWorld()->NewObject<CTransform>();
		Transform->SetSpeed(0.001f);

		Graphics::TBufferDesc VertexConstBufferDesc;
		VertexConstBufferDesc.ByteWidth = sizeof(Transform->GetModelMatrix());
		VertexConstBufferDesc.Usage = Graphics::EUsage::UsageDynamic;
		VertexConstBufferDesc.BindFlags = Graphics::EBindFlags::BindConstantBuffer;
		VertexConstBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessWrite;

		Graphics::TBufferMapResource VertexConstBufferMapResource;
		VertexConstBufferMapResource.MapDataPoint = &Transform->GetModelMatrix();
		VertexConstBufferMapResource.DataSize = sizeof(Transform->GetModelMatrix());

		RenderComponent->AddVertexConstBuffer(VertexConstBufferDesc, VertexConstBufferMapResource);
		std::cout << "CO\n";
	}
	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);
		Transform->Move(Vector3(1.0f, 0.0f, 0.0f));
		Transform->CalculateModelMatrix();
		RenderComponent->UpdateVertexConstBuffer(0);
	}
};

CWorld::CWorld()
	: bFlagDestroyedWorldObject(false)
{
	CO* co = SpawnActor<CO>();
}

CWorld::~CWorld()
{
	
}
