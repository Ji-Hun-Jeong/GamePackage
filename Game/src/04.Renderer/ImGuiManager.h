#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "06.Interaction/MouseInteracter.h"

class CImGuiManager
{
	SINGLE(CImGuiManager)
public:
	void StartFrame(const std::string& InMainPanelName)
	{
		MainPanelName = InMainPanelName;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin(MainPanelName.c_str());

		ImGuiIO& io = ImGui::GetIO();
		ImGui::Text("Delta Time: %.6f seconds", io.DeltaTime);
	}
	void EndFrame()
	{
		ImGui::End();
	}
	void Render()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	
	void DeliverMouseInteraction(class CMouseInteractionManager& InMouseInteractionManager, uint32_t InScreenWidth, uint32_t InScreenHeight);
	const std::string& GetMainPanelName() const { return MainPanelName; }

private:
	Vector2 Position;
	Vector2 Size;

	CMouseInteracter MouseInteracter;

	std::string MainPanelName;
};

