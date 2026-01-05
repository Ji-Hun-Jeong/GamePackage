#include "pch.h"
#include "CollisionManager.h"
#include <algorithm>

bool CCollisionManager::IsCollision(CRectCollider& InRectCollider1, CRectCollider& InRectCollider2)
{
    // 리팩토링된 최신 위치 가져오기
    const Vector2 Pos1 = InRectCollider1.GetFinalPosition();
    const Vector2 Pos2 = InRectCollider2.GetFinalPosition();

    float dx = std::abs(Pos1.x - Pos2.x);
    float dy = std::abs(Pos1.y - Pos2.y);

    // Collider의 Scale이 '전체 크기'라면 0.5f를 곱해주는 과정이 필요할 수 있습니다.
    const Vector2 Half1 = InRectCollider1.GetRectScale() * 0.5f;
    const Vector2 Half2 = InRectCollider2.GetRectScale() * 0.5f;

    return (dx <= (Half1.x + Half2.x)) && (dy <= (Half1.y + Half2.y));
}

bool CCollisionManager::IsCollision(CRectCollider& InRectCollider, CCircleCollider& InCircleCollider)
{
    // 1. 필요한 변수들 가져오기
    Vector2 rectPos = InRectCollider.GetFinalPosition();
    Vector2 rectScale = InRectCollider.GetRectScale(); // {halfWidth, halfHeight}
    Vector2 circlePos = InCircleCollider.GetFinalPosition();
    float circleRadius = InCircleCollider.GetRadius();

    // 2. 사각형 중심에서 원 중심까지의 벡터
    Vector2 distVec = circlePos - rectPos;

    // 3. 사각형의 범위(extents) 내에서 원의 중심과 가장 가까운 점(x, y) 찾기
    //    (distVec를 사각형의 절반 크기로 제한(clamp)합니다)
    float closestX = std::max<float>(-rectScale.x, std::min<float>(distVec.x, rectScale.x));
    float closestY = std::max<float>(-rectScale.y, std::min<float>(distVec.y, rectScale.y));

    // 4. 사각형 위의 "가장 가까운 점"과 "원 중심" 사이의 벡터
    Vector2 delta = distVec - Vector2(closestX, closestY);

    // 5. 이 거리(제곱)가 원의 반지름(제곱)보다 작으면 충돌
    float distSq = (delta.x * delta.x) + (delta.y * delta.y);

    return distSq <= (circleRadius * circleRadius);
}

bool CCollisionManager::IsCollision(CCircleCollider& InCircleCollider1, CCircleCollider& InCircleCollider2)
{
    return false;
}
