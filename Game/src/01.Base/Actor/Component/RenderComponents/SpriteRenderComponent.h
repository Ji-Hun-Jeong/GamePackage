#pragma once
#include "RenderComponent.h"

class CSpriteRenderComponent : public CRenderComponent
{
	GENERATE_OBJECT(CSpriteRenderComponent)
public:
	CSpriteRenderComponent();
	~CSpriteRenderComponent() = default;

public:
	void Render(CSpriteRenderer& InRenderer) override;

	void SetMesh(const Graphics::TMeshData& InMeshData);
	void SetDiffuseImage(const std::wstring& InImagePath);
	void SetPSO(EPSOType InPSOType);

	void SetLayer(uint32_t InLayer) { RenderStateObject.SetRenderLayer(InLayer); }
	void SetRender(bool bInRender) { bRender = bInRender; }

	void UpdateModelToNDC(const Vector3& InPosition, const Vector3& InRotation, const Vector3& InScale
		, uint32_t InScreenWidth, uint32_t InScreenHeight)
	{
		Vector3 NormalizedScale = Vector3(
			InScale.x / InScreenWidth,
			InScale.y / InScreenHeight,
			InScale.z // Z축은 보통 그대로 둠
		);
		float NormalizedX = (InPosition.x / (InScreenWidth * 0.5f));
		float NormalizedY = (InPosition.y / (InScreenHeight * 0.5f));

		Scale = NormalizedScale;
		Rotation = InRotation;
		Position = Vector3(NormalizedX, NormalizedY, InPosition.z);

		bUpdatedModel = true;
	}
	void SetColor(const Vector3& InColor, float InAlpha);
	void SetEdge(const Vector3& InEdgeColor, uint32_t InEdgeRange, float InWidth, float InHeight);

	bool IsImageType() const { return Image; }
	Vector2 GetImageScale() const { return Vector2(ImageScale.x, ImageScale.y); }
	const std::wstring& GetImagePath() const { return Image->GetImagePath(); }

private:
	void ClearState()
	{
		bUpdatedImage = false;
		bUpdatedModel = false;
		bUpdatedColor = false;
		bUpdatedEdge = false;
	}

private:
	Graphics::TMeshData MeshData;

	EPSOType PSOType;

	CImage* Image;
	Matrix ImageMatrix;
	bool bUpdatedImage;

	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;
	Vector3 ImageScale;
	bool bUpdatedModel;

	struct TColorData
	{
		Vector3 Color = { 0.0f,0.0f,0.0f };
		float Alpha = 1.0f;
	} ColorData;
	static_assert(sizeof(TColorData) % 16 == 0);
	bool bUpdatedColor;

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

