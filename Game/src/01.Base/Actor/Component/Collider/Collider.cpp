#include "pch.h"
#include "Collider.h"

//bool CRectCollider::IsCollision(CRectCollider& InRectCollisionObject)
//{
//    // 1. 두 중심점 사이의 X, Y축 거리(절댓값)를 계산
//    float dx = std::abs(CenterPosition.x - InRectCollisionObject.GetCenterPosition().x);
//    float dy = std::abs(CenterPosition.y - InRectCollisionObject.GetCenterPosition().y);
//
//    // 2. 두 사각형의 절반 너비(x)와 절반 높이(y)의 합을 계산
//    float sumHalfWidths = this->RectScale.x + InRectCollisionObject.RectScale.x;
//    float sumHalfHeights = this->RectScale.y + InRectCollisionObject.RectScale.y;
//
//    // 3. X축 *그리고* Y축 모두에서 겹치면 충돌
//    return (dx <= sumHalfWidths) && (dy <= sumHalfHeights);
//}
//
//bool CRectCollider::IsCollision(CCircleCollider& InCircleCollisionObject)
//{
//    // 1. 필요한 변수들 가져오기
//    Vector2 rectPos = this->CenterPosition;
//    Vector2 rectScale = this->RectScale; // {halfWidth, halfHeight}
//    Vector2 circlePos = InCircleCollisionObject.GetCenterPosition();
//    float circleRadius = InCircleCollisionObject.GetRadius();
//
//    // 2. 사각형 중심에서 원 중심까지의 벡터
//    Vector2 distVec = circlePos - rectPos;
//
//    // 3. 사각형의 범위(extents) 내에서 원의 중심과 가장 가까운 점(x, y) 찾기
//    //    (distVec를 사각형의 절반 크기로 제한(clamp)합니다)
//    float closestX = std::max(-rectScale.x, std::min(distVec.x, rectScale.x));
//    float closestY = std::max(-rectScale.y, std::min(distVec.y, rectScale.y));
//
//    // 4. 사각형 위의 "가장 가까운 점"과 "원 중심" 사이의 벡터
//    Vector2 delta = distVec - Vector2(closestX, closestY);
//
//    // 5. 이 거리(제곱)가 원의 반지름(제곱)보다 작으면 충돌
//    float distSq = (delta.x * delta.x) + (delta.y * delta.y);
//
//    return distSq <= (circleRadius * circleRadius);
//}
//
//bool CCircleCollider::IsCollision(CRectCollider& InRectCollisionObject)
//{
//	return false;
//}
//
//bool CCircleCollider::IsCollision(CCircleCollider& InCircleCollisionObject)
//{
//    // 1. 두 반지름의 합
//    float sumRadii = this->Radius + InCircleCollisionObject.Radius;
//
//    // 2. 두 중심점 사이의 거리 (제곱)
//    float dx = this->CenterPosition.x - InCircleCollisionObject.GetCenterPosition().x;
//    float dy = this->CenterPosition.y - InCircleCollisionObject.GetCenterPosition().y;
//    float distSq = (dx * dx) + (dy * dy);
//
//    // 3. 제곱 거리와 제곱 반지름 합을 비교
//    return distSq <= (sumRadii * sumRadii);
//}
