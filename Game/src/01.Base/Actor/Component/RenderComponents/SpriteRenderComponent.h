#pragma once
#include "RenderComponent.h"

class CSpriteRenderComponent : public CRenderComponent
{
	GENERATE_OBJECT(CSpriteRenderComponent)
public:
	CSpriteRenderComponent()
		: PSOType(EPSOType::Basic)
		, bUpdateMesh(false)
		, bUpdatePSO(false)
		, bUpdatedImage(false)
		, bUpdatedModel(false)
		, bUpdatedColor(false)
		, Scale(Vector3(1.0f))
	{}
	~CSpriteRenderComponent() = default;

public:
	void SetMesh(const Graphics::TMeshData& InMeshData)
	{
		MeshData = InMeshData;
		bUpdateMesh = true;
	}

	void SetPSO(EPSOType InPSOType)
	{
		PSOType = InPSOType;
		bUpdatePSO = true;
	}

	void SetDiffuseImage(const std::wstring& InImagePath)
	{
		ImagePath = InImagePath;
		bUpdatedImage = true;
	}

	void SetLayer(uint32_t InLayer) { RenderStateObject.SetRenderLayer(InLayer); }

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
	void UpdateColor(const Vector3& InColor, float InAlpha)
	{
		ColorData.Color = InColor;
		ColorData.Transparency = InAlpha;
		bUpdatedColor = true;
	}
	void Render(CSpriteRenderer& InRenderer) override
	{
		if (bUpdateMesh)
			InRenderer.SetMeshToRSO(RenderStateObject, MeshData);
		if (bUpdatePSO)
			InRenderer.SetPSOToRSO(RenderStateObject, PSOType);

		if (bUpdatedImage)
		{
			CImage* Image = InRenderer.SetShaderResourceToRSOByImagePath(RenderStateObject, ImagePath);
			const auto& Texture2DDesc = Image->GetTexture2D().GetTexture2DDesc();
			ImageScale = Vector3(float(Texture2DDesc.Width), float(Texture2DDesc.Height), 1.0f);
		}
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
	bool IsImageType() const { return ImagePath.empty() == false; }
	Vector2 GetImageScale() const { return Vector2(ImageScale.x, ImageScale.y); }
	const std::wstring& GetImagePath() const { return ImagePath; }

private:
	void ClearState()
	{
		bUpdateMesh = false;
		bUpdatePSO = false;
		bUpdatedImage = false;
		bUpdatedModel = false;
		bUpdatedColor = false;
	}

private:
	Graphics::TMeshData MeshData;
	bool bUpdateMesh;

	EPSOType PSOType;
	bool bUpdatePSO;

	std::wstring ImagePath;
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
};

