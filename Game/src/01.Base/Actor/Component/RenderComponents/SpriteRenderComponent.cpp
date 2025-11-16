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
	: PSOType(EPSOType::Basic)
	, Image(nullptr)
	, bUpdatedImage(false)
	, bUpdatedModel(false)
	, bUpdatedColor(false)
	, Scale(Vector3(1.0f))
{
	RenderStateObject.MountConstBuffer(EShaderType::VertexShader, 0
		, CRenderResourceLoader::GetInst().CreateConstBuffer(sizeof(Matrix)));

	static CImageMeshDataLoader ImageMeshDataLoader;
}
void CSpriteRenderComponent::SetMesh(const Graphics::TMeshData& InMeshData)
{
	Graphics::CMesh* Mesh = CRenderResourceLoader::GetInst().LoadMesh(InMeshData);
	RenderStateObject.MountMesh(Mesh);
	MeshData = InMeshData;
}
void CSpriteRenderComponent::SetDiffuseImage(const std::wstring& InImagePath)
{
	Image = CRenderResourceLoader::GetInst().LoadImageFromFile(InImagePath);
	RenderStateObject.MountShaderResource(EShaderType::PixelShader, 0, &Image->GetSRV());
	const auto& Texture2DDesc = Image->GetTexture2D().GetTexture2DDesc();
	ImageScale = Vector3(float(Texture2DDesc.Width), float(Texture2DDesc.Height), 1.0f);
	bUpdatedImage = true;
}

void CSpriteRenderComponent::SetPSO(EPSOType InPSOType)
{
	CPSO* PSO = CPSOManager::GetInst().GetPSO(InPSOType);
	RenderStateObject.MountPSO(PSO);

	PSOType = InPSOType;
}

void CSpriteRenderComponent::UpdateColor(const Vector3& InColor, float InAlpha)
{
	ColorData.Color = InColor;
	ColorData.Transparency = InAlpha;
	bUpdatedColor = true;

	if (RenderStateObject.IsExistConstBufferInSlot(EShaderType::PixelShader, 0) == false)
	{
		RenderStateObject.MountConstBuffer(EShaderType::PixelShader, 0
			, CRenderResourceLoader::GetInst().CreateConstBuffer(sizeof(ColorData)));
	}
}

void CSpriteRenderComponent::Render(CSpriteRenderer& InRenderer)
{
	if (bUpdatedModel || bUpdatedImage)
	{
		Vector3 FinalScale = Scale;
		if (IsImageType())
			FinalScale *= ImageScale;
		Matrix ModelMatrix = (Matrix::CreateScale(FinalScale)
			* Matrix::CreateRotationX(Rotation.x)
			* Matrix::CreateRotationY(Rotation.y)
			* Matrix::CreateRotationZ(Rotation.z)
			* Matrix::CreateTranslation(Position)).Transpose();

		InRenderer.UpdateConstBuffer(RenderStateObject, EShaderType::VertexShader, 0, &ModelMatrix, sizeof(ModelMatrix));
	}

	if (bUpdatedColor)
		InRenderer.UpdateConstBuffer(RenderStateObject, EShaderType::PixelShader, 0, &ColorData, sizeof(ColorData));

	InRenderer.RenderObject(RenderStateObject);

	ClearState();
}