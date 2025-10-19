#include "pch.h"
#include "UI.h"

#include "01.Base/World/World.h"

void CUI::Initalize()
{
	CActor::Initalize();

	SetRenderComponent();

	SetInteractionComponent();
}
