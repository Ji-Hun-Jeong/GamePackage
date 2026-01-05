#include "pch.h"
#include "Transform.h"

#include "01.Base/Actor/Actor.h"
const Vector3& CTransform::GetWorldPosition()
{
    if (bChangePosition)
    {
        CActor* OwnerParentActor = GetOwnerActor()->GetOwner();
        if (OwnerParentActor)
            WorldPosition = OwnerParentActor->GetTransform()->GetWorldPosition() + Position;
        else
            WorldPosition = Position;

        bChangePosition = false;
    }

    return WorldPosition;
}

Vector2 CTransform::GetWorldPosition2D()
{
    GetWorldPosition();
    return Vector2(WorldPosition.x, WorldPosition.y);
}

void CTransform::SetPosition(const Vector3& InPosition)
{
    if (Position != InPosition)
    {
        Position = InPosition;
        bChangePosition = true;

        ChangePosition();
    }
}

void CTransform::ChangePosition()
{
    bChangePosition = true;

    CActor* OwnerActor = GetOwnerActor();
    const std::vector<CActor*>& Childs = OwnerActor->GetChild();
    for (auto Child : Childs)
        Child->GetTransform()->ChangePosition();
}
