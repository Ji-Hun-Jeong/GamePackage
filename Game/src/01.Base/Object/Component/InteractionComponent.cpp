#include "pch.h"
#include "InteractionComponent.h"
#include "01.Base/Object/Actor.h"

void CInteractionComponent::SetMouseManager(CMouseManager* InMouseManager)
{
	assert(InMouseManager);
	MouseManager = InMouseManager;
	CMouseInteracter* OwnerInteracter = nullptr;
	CActor* Owner = GetOwner();
	if (Owner && Owner->GetInteractionComponent())
		OwnerInteracter = Owner->GetInteractionComponent()->MouseInteracter;

	MouseInteracter = MouseManager->NewMouseInteracter(OwnerInteracter);
}