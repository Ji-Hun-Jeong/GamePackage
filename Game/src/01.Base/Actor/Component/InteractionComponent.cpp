#include "pch.h"
#include "InteractionComponent.h"
#include "01.Base/Actor/Actor.h"
#include "06.Interaction/MouseInteractionManager.h"

void CInteractionComponent::InteractionCheck(CMouseInteractionManager& InMouseInteractionManager)
{
	if (bInteraction == false)
		return;
	if(bEndInteractionCheck)
		InMouseInteractionManager.PushFocusInteracterToEnd(&MouseInteracter);
	else
		InMouseInteractionManager.PushFocusInteracter(&MouseInteracter);
}
