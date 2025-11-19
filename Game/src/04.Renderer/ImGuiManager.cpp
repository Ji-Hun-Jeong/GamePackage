#include "pch.h"
#include "ImGuiManager.h"

#include "06.Interaction/MouseInteractionManager.h"

CImGuiManager::CImGuiManager()
{

}

void CImGuiManager::DeliverMouseInteraction(CMouseInteractionManager& InMouseInteractionManager, uint32_t InScreenWidth, uint32_t InScreenHeight)
{
	Vector2 ImGuiCurrentSize = Vector2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	Vector2 ImGuiCurrentPosition = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y) + ImGuiCurrentSize / 2.0f;
	ImGuiCurrentPosition.x -= float(InScreenWidth) / 2.0f;
	ImGuiCurrentPosition.y = -ImGuiCurrentPosition.y + float(InScreenHeight) / 2.0f;
	ImGuiCurrentPosition += Offset;

	if ((ImGuiCurrentPosition.x != Position.x) || (ImGuiCurrentPosition.y != Position.y)
		|| (ImGuiCurrentSize.x != Size.x) || (ImGuiCurrentSize.y != Size.y))
	{
		Position.x = ImGuiCurrentPosition.x;
		Position.y = ImGuiCurrentPosition.y;
		Size.x = ImGuiCurrentSize.x;
		Size.y = ImGuiCurrentSize.y;

		MouseInteracter.SetPosition(Position);
		MouseInteracter.SetSize(Size);
	}
	InMouseInteractionManager.PushFocusInteracterToEnd(&MouseInteracter);
}
