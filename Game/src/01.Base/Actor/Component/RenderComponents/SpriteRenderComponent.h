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
	void UpdateColor(const Vector3& InColor, float InAlpha);

	bool IsImageType() const { return Image; }
	Vector2 GetImageScale() const { return Vector2(ImageScale.x, ImageScale.y); }
	const std::wstring& GetImagePath() const { return Image->GetImagePath(); }

private:
	void ClearState()
	{
		bUpdatedImage = false;
		bUpdatedModel = false;
		bUpdatedColor = false;
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
		Vector3 Color{ 0.0f,0.0f,0.0f };
		float Transparency = 0.0f;
	} ColorData;
	bool bUpdatedColor;

	bool bRender;
};

