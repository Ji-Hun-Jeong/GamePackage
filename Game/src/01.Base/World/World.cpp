#include "pch.h"
#include "World.h"

class CIO : public CActor
{
	GenerateObject()
public:
	void Initalize() override
	{
		CActor::Initalize();

		struct Vertex
		{
			Vector3 Position;
			Vector2 Uv;
		};

		std::vector<Vertex> Vertices =
		{
			{Vector3(-1.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)},
			{Vector3(-1.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)},
			{Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)},
			{Vector3(1.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)}
		};

		Graphics::TMeshData MeshData;
		MeshData.VertexBufferDesc.BindFlags = Graphics::EBindFlags::BindVertexBuffer;
		MeshData.VertexBufferDesc.ByteWidth = uint32_t(sizeof(Vertex) * Vertices.size());
		MeshData.VertexBufferDesc.CPUAccessFlags = Graphics::ECPUAccessFlags::CpuAccessImpossible;
		MeshData.VertexBufferDesc.Usage = Graphics::EUsage::UsageImmutable;

		MeshData.VertexBufferInitData.CopyStartPoint = Vertices.data();

		std::vector<uint32_t> Indices =
		{
			0, 1, 2, 0, 2, 3
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
		MeshData.Key = 1;

		Graphics::TMaterialData MaterialData;
		MaterialData.ImagePath = L"resources/image/Player/Alert/0.png";
		MaterialData.Key = 0;
		SetRenderComponent();

		RenderComponent->SetMesh(MeshData);
		RenderComponent->SetMaterial(MaterialData);
		RenderComponent->SetPSO(EPSOType::Basic);

		Transform->SetScale(Vector3(0.3f));
		Transform->SetSpeed(0.001f);
	}

	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);
		Transform->Move(Vector3(-1.0f, 0.0f, 0.0f));
	}
};

CWorld::CWorld()
	: bFlagDestroyedWorldObject(false)
{
	
}

CWorld::~CWorld()
{
	
}

void CWorld::Start()
{
	CIO* co = SpawnActor<CIO>();
}
