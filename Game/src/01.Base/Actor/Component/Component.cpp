#include "pch.h"
#include "Component.h"

#include "01.Base/Actor/Actor.h"

CComponent::~CComponent()
{

}

CWorld* CComponent::GetWorld() const
{
    return OwnerActor->GetWorld();
}
