#pragma once
#include "IEditState.h"

class CTileEditState : public CEditState
{
	GENERATE_OBJECT(CTileEditState)
public:
	CTileEditState();
	~CTileEditState() = default;

public:
	void InitalizeEditState(TTileMapEditContext& InTileMapEditContext) override;
	void EnterEditState() override
	{
		CUI* MainPanel = GetTileMapEditContext().MainPanel;
		CTileMap* TileMap = GetTileMapEditContext().TileMap;

		MoveUIOwner->Activate(false);
		MainPanel->AttachChildUI(*MoveUIOwner);

		MainPanel->SetMouseFocusEvent([this]()->void
			{
				bOnMainPanel = true;
			});
	}
	void OnEditState() override;

	void ExitEditState() override
	{
		CUI* MainPanel = GetTileMapEditContext().MainPanel;
		CTileMap* TileMap = GetTileMapEditContext().TileMap;
		CTileMapper& TileMapper = GetTileMapEditContext().TileMapper;
		CTileFocus* TileFocus = GetTileMapEditContext().TileFocus;
		CTileHandler* TileHandler = GetTileMapEditContext().TileHandler;

		MainPanel->SetMouseFocusEvent(nullptr);

		MoveUIOwner->Activate(false);
		MainPanel->DetachChildUI(*MoveUIOwner);

		TileFocus->SetFocusTile(nullptr, 0);
		TileHandler->ClearHandledTiles();
	}
	void ToImGUI() override
	{
		if (ImGui::Button("OpenWindowDialog"))
			ImageImporter.AddImagePathByWindowManager(CWindowManager::GetInst());

		ImGui::BeginChild("TileList", ImVec2(0, 0), true);

		const std::wstring& GeneratedActorImagePath = ImageImporter.GetCurrentImagePath();
		for (auto& Pair : ImageImporter.GetLoadedImagePaths())
		{
			const std::string& ImageName = Pair.first;
			const std::wstring& ImagePath = Pair.second;

			bool bSelected = (GeneratedActorImagePath == ImagePath);

			if (ImGui::Selectable(ImageName.c_str(), bSelected))
			{
				ImageImporter.SetCurrentImagePath(ImageName);
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndChild();
	}

private:
	void InitalizeMoveUI(CUI* InMoveUI)
	{
		InMoveUI->Activate(false);
		InMoveUI->SetLineActor();
		InMoveUI->GetTransform()->SetScale(Vector3(20.0f, 20.0f, 1.0f));
		MoveUIOwner->AttachChildUI(*InMoveUI);
	}

private:
	CImageImporter ImageImporter;
	bool bOpenWindowDialog = false;

	CUI* MoveUIOwner = nullptr;

	bool bOnMainPanel = false;

};

