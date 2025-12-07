#include "pch.h"
#include "ImGuiManager.h"

#include "04.Renderer/SpriteRenderer.h"
#include "06.Interaction/MouseInteractionManager.h"

CImGuiManager::CImGuiManager()
{

}

void CImGuiManager::DeliverMouseInteraction(CMouseInteractionManager& InMouseInteractionManager, CSpriteRenderer& InRenderer)
{
	Vector2 ImGuiCurrentSize = Vector2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	Vector2 ImGuiCurrentPosition = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y) + ImGuiCurrentSize / 2.0f;
	ImGuiCurrentPosition.x -= float(InRenderer.GetScreenWidth()) / 2.0f;
	ImGuiCurrentPosition.y = -ImGuiCurrentPosition.y + float(InRenderer.GetScreenHeight()) / 2.0f;
	ImGuiCurrentPosition += Vector2(InRenderer.GetRendererWorldPosition().x, InRenderer.GetRendererWorldPosition().y);

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
