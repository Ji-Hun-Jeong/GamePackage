#include "pch.h"
#include "Component.h"
#include "01.Base/World/World.h"
#include "01.Base/Object/Actor.h"

CComponent::~CComponent()
{

}

void CComponent::SetOwner(CActor* InOwnerActor)
{
	InOwnerActor->Attach(this);
}

void CComponent::Destroy()
{
	GetWorld()->PushWorldSynchronizeEvent([this]()->void
		{
			bDestroy = true;
			EndPlay();
		});
}
