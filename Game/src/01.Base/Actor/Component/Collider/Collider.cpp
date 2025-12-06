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
	SpriteData.Color = Vector3(1.0f, 0.0f, 0.0f);
	SpriteData.Alpha = 1.0f;
	Graphics::CBuffer& ColorBuffer = InRenderer.EntrustBuffer(&SpriteData, sizeof(SpriteData));

	InRenderer.StartState();

	InRenderer.DrawMesh(*Mesh, Vector3(FinalPosition.x, FinalPosition.y, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(RectScale.x, RectScale.y, 1.0f)
		, PSO, *Material, 1);
	InRenderer.SetInstanceData(EShaderType::PixelShader, 0, ColorBuffer);

	InRenderer.EndState();
}

CCircleCollider::CCircleCollider()
	: CCollider(EColliderType::Circle)
	, Radius(0.0f)
{}