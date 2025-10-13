#include "pch.h"
#include "World.h"

class CCO : public CObject
{
public:
	void BeginPlay() override
	{
		CObject::BeginPlay();
		std::cout << "CCO\n";
	}
	void Update(float InDeltaTime) override
	{
		CObject::Update(InDeltaTime);
	}
};

class CO : public CObject
{
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
		MeshData.PrimitiveTopology = Graphics::ETopology::PrimitiveTopologyTRIANGLELIST;
		MeshData.IndexFormat = Graphics::EGIFormat::GI_FORMAT_R32_UINT;
		MeshData.IndexCount = uint32_t(Indices.size());
		MeshData.Stride = sizeof(Vertex);
		MeshData.Offset = 0;

		Graphics::TMaterialData MaterialData;

		CModel* Model = CAssetLoader::GetInst().MakeModel("Triangle", MeshData, MaterialData);

		CPSO* BasicPSO = CPSOManager::GetInst().GetPSO("BasicPSO");
		RenderComponent = std::make_unique<CRenderComponent>(new CRenderStateObject(&Model->GetMesh(), &Model->GetMaterial(), BasicPSO));
		std::cout << "CO\n";
	}
	void Update(float InDeltaTime) override
	{
		CObject::Update(InDeltaTime);
	}
};

CWorld::CWorld()
	: bFlagDestroyedWorldObject(false)
{
	CO* co = NewObject<CO>();
}

CWorld::~CWorld()
{
	
}
