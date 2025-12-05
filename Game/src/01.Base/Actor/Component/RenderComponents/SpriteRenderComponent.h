#pragma once
#include "RenderComponent.h"

class CSpriteRenderComponent : public CRenderComponent
{
	GENERATE_OBJECT(CSpriteRenderComponent)
public:
	CSpriteRenderComponent();
	~CSpriteRenderComponent() = default;

public:
	void Render(CSpriteRenderer& InRenderer, const Vector3& InPosition, const Vector3& InRotation, const Vector3& InScale) override;

	void SetMesh(const Graphics::TMeshData& InMeshData);
	void SetDiffuseImage(const std::wstring& InImagePath);
	/*void SetPSO(EPSOType InPSOType);*/

	void SetLayer(uint32_t InLayer) { RenderLayer = InLayer; }
	void SetRender(bool bInRender) { bRender = bInRender; }

	void SetColor(const Vector3& InColor, float InAlpha);
	void SetEdge(const Vector3& InEdgeColor, uint32_t InEdgeRange, float InWidth, float InHeight);

	bool IsImageType() const { return !MaterialData.ImagePaths[0].empty(); }
	Vector2 GetImageScale() const { return Vector2(ImageScale.x, ImageScale.y); }
	const std::wstring& GetImagePath() const { return ImagePath; }

private:
	void ClearState()
	{
		bUpdatedImage = false;
		bUpdatedColor = false;
		bUpdatedEdge = false;
	}

private:
	Graphics::TMeshData MeshData;
	Graphics::CMesh* Mesh = nullptr;

	Graphics::TMaterialData MaterialData;
	Graphics::CMaterial* Material = nullptr;
	Vector3 ImageScale;
	std::wstring ImagePath;
	bool bUpdatedImage;

	uint32_t RenderLayer = 0;

	std::unique_ptr<Graphics::CBuffer> ColorBuffer = nullptr;
	struct TColorData
	{
		Vector3 Color = { 0.0f,0.0f,0.0f };
		float Alpha = 1.0f;
	} ColorData;
	static_assert(sizeof(TColorData) % 16 == 0);
	bool bUpdatedColor;

	std::unique_ptr<Graphics::CBuffer> EdgeBuffer = nullptr;
	struct TEdgeData
	{
		Vector3 EdgeColor = { 0.0f,0.0f,0.0f };
		uint32_t EdgeRange = 0;

		float Width = 0.0f;
		float Height = 0.0f;
		uint32_t UseImage = 0;
		uint32_t Dummy = 0;
	} EdgeData;
	static_assert(sizeof(TEdgeData) % 16 == 0);
	bool bUpdatedEdge;

	bool bRender;
};

