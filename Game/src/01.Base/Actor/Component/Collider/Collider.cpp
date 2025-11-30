#include "pch.h"
#include "Collider.h"

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

}

CCircleCollider::CCircleCollider()
	: CCollider(EColliderType::Circle)
	, Radius(0.0f)
{}