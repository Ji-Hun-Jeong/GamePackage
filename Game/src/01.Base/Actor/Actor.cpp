#include "pch.h"
#include "Actor.h"
#include "03.Utils/GeometryGenerator.h"

class CImageMeshDataLoader
{
public:
	CImageMeshDataLoader()
	{
		TGeometryData GeometryData = CGeometryGenerator::GenerateSquare();

		Graphics::TMeshData MeshData;

		size_t VertexBufferByteWidth = sizeof(TVertex) * GeometryData.Vertices.size();
		MeshData.Vertices.resize(VertexBufferByteWidth);
		memcpy(MeshData.Vertices.data(), GeometryData.Vertices.data(), VertexBufferByteWidth);

		size_t IndexBufferByteWidth = sizeof(uint32_t) * GeometryData.Indices.size();
		MeshData.Indices.resize(IndexBufferByteWidth);
		memcpy(MeshData.Indices.data(), GeometryData.Indices.data(), IndexBufferByteWidth);

		MeshData.IndexFormat = Graphics::EGIFormat::GI_FORMAT_R32_UINT;
		MeshData.IndexCount = uint32_t(GeometryData.Indices.size());
		MeshData.Stride = sizeof(TVertex);
		MeshData.Offset = 0;
		MeshData.Key = 0;

		CAssetLoader::GetInst().LoadMeshData("ImageMesh", MeshData);
	}
};

CActor::CActor()
	: Owner(nullptr)
	, bActive(true)
	, Transform(nullptr)
	, InteractionComponent(nullptr)
{
	Transform = AddComponent<CTransform>();

	static CImageMeshDataLoader ImageMeshDataLoader;
}