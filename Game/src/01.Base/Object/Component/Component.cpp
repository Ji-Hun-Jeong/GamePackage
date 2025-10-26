#include "pch.h"
#include "Component.h"
#include "01.Base/Object/Actor.h"

CComponent::~CComponent()
{

}

void CComponent::ResetOwner(CActor* InOwnerActor)
{
	InOwnerActor->Detach(this);
}

void CComponent::SetOwner(CActor* InOwnerActor)
{
	InOwnerActor->Attach(this);
}

void CComponent::Destroy()
{
	GetWorld()->DestroyObject(this);
}
