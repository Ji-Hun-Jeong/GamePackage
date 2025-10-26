#include "pch.h"
#include "InteractionComponent.h"
#include "01.Base/Object/Actor.h"

void CInteractionComponent::SetMouseManager(CMouseManager* InMouseManager)
{
	assert(InMouseManager);
	MouseManager = InMouseManager;
	CMouseInteracter* OwnerInteracter = nullptr;
	CActor* OwnerActor = GetOwner()->GetOwner();
	if (OwnerActor && OwnerActor->GetInteractionComponent())
		OwnerInteracter = OwnerActor->GetInteractionComponent()->MouseInteracter;

	MouseInteracter = MouseManager->NewMouseInteracter(OwnerInteracter);
}