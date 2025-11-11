#include "pch.h"
#include "CollisionManager.h"
#include <algorithm>

bool CCollisionManager::IsCollision(CRectCollider& InRectCollider1, CRectCollider& InRectCollider2)
{
	// 1. 두 중심점 사이의 X, Y축 거리(절댓값)를 계산
    float dx = std::abs(InRectCollider1.GetCenterPosition().x - InRectCollider2.GetCenterPosition().x);
    float dy = std::abs(InRectCollider1.GetCenterPosition().y - InRectCollider2.GetCenterPosition().y);

    // 2. 두 사각형의 절반 너비(x)와 절반 높이(y)의 합을 계산
    float sumHalfWidths = InRectCollider1.GetRectScale().x + InRectCollider2.GetRectScale().x;
    float sumHalfHeights = InRectCollider1.GetRectScale().y + InRectCollider2.GetRectScale().y;

    return (dx <= sumHalfWidths) && (dy <= sumHalfHeights);
}

bool CCollisionManager::IsCollision(CRectCollider& InRectCollider, CCircleCollider& InCircleCollider)
{
    // 1. 필요한 변수들 가져오기
    Vector2 rectPos = InRectCollider.GetCenterPosition();
    Vector2 rectScale = InRectCollider.GetRectScale(); // {halfWidth, halfHeight}
    Vector2 circlePos = InCircleCollider.GetCenterPosition();
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
