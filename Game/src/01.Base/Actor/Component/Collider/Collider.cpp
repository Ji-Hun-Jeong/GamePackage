#include "pch.h"
#include "Collider.h"

#include "03.Utils/AssetLoader.h"
#include "04.Renderer/RenderResourceLoader.h"
#include "04.Renderer/SpriteRenderer.h"

CCollider::CCollider(EColliderType InColliderType)
	: ColliderType(InColliderType)
	, bDebugRender(false)
{
	RenderStateObject.MountConstBuffer(EShaderType::VertexShader, 0
		, CRenderResourceLoader::GetInst().CreateConstBuffer(sizeof(Matrix)));
}

CRectCollider::CRectCollider()
	: CCollider(EColliderType::Rect)
{
	Graphics::CMesh* Mesh = CRenderResourceLoader::GetInst().LoadMesh(CAssetLoader::GetInst().GetMeshData("ImageMesh"));
	RenderStateObject.MountMesh(Mesh);
	RenderStateObject.MountPSO(CPSOManager::GetInst().GetPSO(EPSOType::Edge));
}

void CRectCollider::DebugRender(CSpriteRenderer& InRenderer)
{
	// 그냥 Renderer에 일회용 그리기 함수 만드는게 나을듯?
	if (bDebugRender == false)
		return;

	Vector2 Position = GetFinalPosition();
	if (InRenderer.IsInsideNDC(Vector2(Position.x, Position.y), Vector2(RectScale.x, RectScale.y)) == false)
		return;

	Matrix ModelMatrix = (Matrix::CreateScale(Vector3(RectScale.x, RectScale.y, 1.0f))
		* Matrix::CreateRotationX(0.0f)
		* Matrix::CreateRotationY(0.0f)
		* Matrix::CreateRotationZ(0.0f)
		* Matrix::CreateTranslation(Vector3(Position.x, Position.y, 0.0f))).Transpose();

	InRenderer.UpdateConstBuffer(RenderStateObject, EShaderType::VertexShader, 0, &ModelMatrix, sizeof(ModelMatrix));

	InRenderer.RenderObject(RenderStateObject);
}

CCircleCollider::CCircleCollider()
	: CCollider(EColliderType::Circle)
	, Radius(0.0f)
{}