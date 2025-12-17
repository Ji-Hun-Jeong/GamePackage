#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "02.Contents/Actor/UI/UI.h"

class CImGUIInteracter : public CUI
{
	GENERATE_OBJECT(CImGUIInteracter)
public:
	CImGUIInteracter()
	{
		SpriteRenderComponent->SetRender(false);
	}
	~CImGUIInteracter() = default;

public:
	void Update(float InDeltaTime) override
	{
		CUI::Update(InDeltaTime);
		
		Vector2 ImGuiCurrentSize = Vector2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		Vector2 ImGuiCurrentPosition = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y) + ImGuiCurrentSize / 2.0f;
		ImGuiCurrentPosition.x -= float(CWindowManager::GetInst().GetScreenWidth()) / 2.0f;
		ImGuiCurrentPosition.y = -ImGuiCurrentPosition.y + float(CWindowManager::GetInst().GetScreenHeight()) / 2.0f;

		Transform->SetScale(Vector3(ImGuiCurrentSize.x, ImGuiCurrentSize.y, 1.0f));
		Transform->SetPosition(Vector3(ImGuiCurrentPosition.x, ImGuiCurrentPosition.y, 0.0f));
	}
private:


};

