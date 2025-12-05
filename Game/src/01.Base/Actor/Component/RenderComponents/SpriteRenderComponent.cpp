#include "pch.h"
#include "SpriteRenderComponent.h"

#include "03.Utils/GeometryGenerator.h"
#include "03.Utils/AssetLoader.h"
#include "04.Renderer/SpriteRenderer.h"
#include "04.Renderer/RenderResourceLoader.h"
#include "04.Renderer/PSOManager.h"
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

		TColorGeometryData ColorGeometryData = CGeometryGenerator::GenerateRect();

		VertexBufferByteWidth = sizeof(TColorVertex) * ColorGeometryData.Vertices.size();
		MeshData.Vertices.resize(VertexBufferByteWidth);
		memcpy(MeshData.Vertices.data(), ColorGeometryData.Vertices.data(), VertexBufferByteWidth);

		IndexBufferByteWidth = sizeof(uint32_t) * ColorGeometryData.Indices.size();
		MeshData.Indices.resize(IndexBufferByteWidth);
		memcpy(MeshData.Indices.data(), ColorGeometryData.Indices.data(), IndexBufferByteWidth);

		MeshData.IndexFormat = Graphics::EGIFormat::GI_FORMAT_R32_UINT;
		MeshData.IndexCount = uint32_t(ColorGeometryData.Indices.size());
		MeshData.Stride = sizeof(TColorVertex);
		MeshData.Offset = 0;
		MeshData.Key = 1;

		CAssetLoader::GetInst().LoadMeshData("RectMesh", MeshData);
	}
};


CSpriteRenderComponent::CSpriteRenderComponent()
	: bUpdatedImage(false)
	, bUpdatedColor(false)
	, bUpdatedEdge(false)
	, bRender(true)
{
	static CImageMeshDataLoader ImageMeshDataLoader;
	MaterialData.PixelShaderPath = L"resources/shader/BasicPixelShader.hlsl";
	Material = CRenderResourceLoader::GetInst().LoadMaterial(MaterialData);
}
void CSpriteRenderComponent::SetMesh(const Graphics::TMeshData& InMeshData)
{
	Mesh = CRenderResourceLoader::GetInst().LoadMesh(InMeshData);
	MeshData = InMeshData;
}
void CSpriteRenderComponent::SetDiffuseImage(const std::wstring& InImagePath)
{
	MaterialData.PixelShaderPath = L"resources/shader/BasicPixelShader.hlsl";
	MaterialData.ImagePaths[0] = InImagePath;
	Material = CRenderResourceLoader::GetInst().LoadMaterial(MaterialData);

	const auto& Texture2DDesc = Material->GetTexture2D(0).GetTexture2DDesc();
	ImageScale = Vector3(float(Texture2DDesc.Width), float(Texture2DDesc.Height), 1.0f);
	bUpdatedImage = true;

	EdgeData.UseImage = true;
}

//void CSpriteRenderComponent::SetPSO(EPSOType InPSOType)
//{
//	PSO = CPSOManager::GetInst().GetPSO(InPSOType);
//	PSOType = InPSOType;
//}

void CSpriteRenderComponent::SetColor(const Vector3& InColor, float InAlpha)
{
	ColorData.Color = InColor;
	ColorData.Alpha = InAlpha;
	bUpdatedColor = true;

	if (ColorBuffer == nullptr)
		ColorBuffer = CRenderResourceLoader::GetInst().CreateConstBuffer(sizeof(ColorData));
}

void CSpriteRenderComponent::SetEdge(const Vector3& InEdgeColor, uint32_t InEdgeRange, float InWidth, float InHeight)
{
	EdgeData.EdgeColor = InEdgeColor;
	EdgeData.EdgeRange = InEdgeRange;
	EdgeData.Width = InWidth;
	EdgeData.Height = InHeight;
	bUpdatedEdge = true;

	if(EdgeBuffer == nullptr)
		EdgeBuffer = CRenderResourceLoader::GetInst().CreateConstBuffer(sizeof(EdgeData));
}

void CSpriteRenderComponent::Render(CSpriteRenderer& InRenderer, const Vector3& InPosition, const Vector3& InRotation, const Vector3& InScale)
{
	if (bRender == false)
		return;

	Vector3 FinalScale = InScale;
	if (IsImageType())
		FinalScale *= ImageScale;

	if(bUpdatedColor)
		InRenderer.UpdateBuffer(*ColorBuffer.get(), &ColorData, sizeof(ColorData));
	if(bUpdatedEdge)
		InRenderer.UpdateBuffer(*EdgeBuffer.get(), &EdgeData, sizeof(EdgeData));
	
	InRenderer.StartState();

	InRenderer.DrawMesh(*Mesh, InPosition, InRotation, FinalScale, *Material, RenderLayer);

	if (ColorBuffer)
		InRenderer.SetInstanceData(EShaderType::PixelShader, 0, *ColorBuffer.get());
	if (EdgeBuffer)
		InRenderer.SetInstanceData(EShaderType::PixelShader, 1, *EdgeBuffer.get());

	InRenderer.EndState();

	ClearState();
}