#include "pch.h"
#include "UIManager.h"

#include "00.App/MouseManager.h"
#include "01.Base/Actor/Camera.h"

void CUIManager::SetMouseWorldPosition(const CMouseManager& InMouseManager, const CCamera& InCamera)
{
	MouseWorldPosition = InMouseManager.GetMousePosition() + InCamera.GetTransform()->GetFinalPosition2D();
}
