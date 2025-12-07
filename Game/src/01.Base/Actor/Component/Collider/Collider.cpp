#include "pch.h"
#include "Collider.h"

#include "../RenderComponents/SpriteRenderComponent.h"
#include "03.Utils/AssetLoader.h"
#include "04.Renderer/RenderResourceLoader.h"
#include "04.Renderer/SpriteRenderer.h"

CCollider::CCollider(EColliderType InColliderType)
	: ColliderType(InColliderType)
	, bDebugRender(false)
{
}

CRectCollider::CRectCollider()
	: CCollider(EColliderType::Rect)
{

}

void CRectCollider::DebugRender(CSpriteRenderer& InRenderer)
{
	if (bDebugRender == false)
		return;

	static const Graphics::TMeshData& MeshData = CAssetLoader::GetInst().GetMeshData("LineSquareMesh");
	static Graphics::CMesh* Mesh = CRenderResourceLoader::GetInst().LoadMesh(MeshData);

	static CPSO* PSO = CPSOManager::GetInst().GetPSO(EPSOType::Line);
	static Graphics::TMaterialData MaterialData;
	MaterialData.PixelShaderPath = L"resources/shader/BasicPixelShader.hlsl";
	static Graphics::CMaterial* Material = CRenderResourceLoader::GetInst().LoadMaterial(MaterialData);
	Vector2 FinalPosition = GetFinalPosition();

	TSpriteData SpriteData;
	SpriteData.Color = Color;
	SpriteData.Alpha = 1.0f;
	Graphics::CBuffer& ColorBuffer = InRenderer.EntrustBuffer(&SpriteData, sizeof(SpriteData));

	InRenderer.StartState();

	InRenderer.DrawMesh(*Mesh, Vector3(FinalPosition.x, FinalPosition.y, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(RectScale.x, RectScale.y, 1.0f)
		, PSO, *Material, Layer);
	InRenderer.SetInstanceData(EShaderType::PixelShader, 0, ColorBuffer);

	InRenderer.EndState();
}

bool CRectCollider::IsInside(const Vector2& InPosition)
{
	float HalfWidth = RectScale.x * 0.5f;
	float HalfHeight = RectScale.y * 0.5f;

	Vector2 FinalPosition = GetFinalPosition();

	float Left = FinalPosition.x - HalfWidth;
	float Right = FinalPosition.x + HalfWidth;
	float Top = FinalPosition.y - HalfHeight;
	float Bottom = FinalPosition.y + HalfHeight;

	if (InPosition.x >= Left && InPosition.x <= Right &&
		InPosition.y >= Top && InPosition.y <= Bottom)
	{
		return true;
	}
	return false;
}

CCircleCollider::CCircleCollider()
	: CCollider(EColliderType::Circle)
	, Radius(0.0f)
{}

bool CCircleCollider::IsInside(const Vector2 & InPosition)
{
	return false;
}
