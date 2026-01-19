#include "pch.h"
#include "UIManager.h"

#include "00.App/MouseManager.h"
#include "00.App/WindowManager.h"
#include "01.Base/Actor/Camera.h"

void CUIManager::SetMouseWorldPosition(const CMouseManager& InMouseManager, const CCamera& InCamera)
{
	CameraPosition = InCamera.GetTransform()->GetPosition();
	MouseWorldPosition = InMouseManager.GetMousePosition() + InCamera.GetTransform()->GetPosition2D();

	bMouseClicked = InMouseManager.IsLButtonHold();
}

bool CUIManager::IsInsideScreen(const CUI& InUI)
{
	Vector3 RelativePosition = InUI.GetTransform()->GetPosition() - CameraPosition;

	float halfW = InUI.GetTransform()->GetScale().x * 0.5f;
	float halfH = InUI.GetTransform()->GetScale().y * 0.5f;

	float left = RelativePosition.x - halfW;
	float right = RelativePosition.x + halfW;
	float top = RelativePosition.y - halfH;
	float bottom = RelativePosition.y + halfH;

	float screenLeft = -float(CWindowManager::GetInst().GetScreenWidth()) * 0.5f;
	float screenRight = float(CWindowManager::GetInst().GetScreenWidth()) * 0.5f;
	float screenTop = -float(CWindowManager::GetInst().GetScreenHeight()) * 0.5f;
	float screenBottom = float(CWindowManager::GetInst().GetScreenHeight()) * 0.5f;

	bool isInside =
		right >= screenLeft &&
		left <= screenRight &&
		bottom >= screenTop &&
		top <= screenBottom;

	return isInside;
}
