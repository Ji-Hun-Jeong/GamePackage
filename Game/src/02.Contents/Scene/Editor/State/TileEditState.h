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
		CTileMap* TileMap = GetTileMapEditContext().TileMap;
		CUI* MainPanel = GetTileMapEditContext().MainPanel;

		MoveUIOwner->Activate(false);
		MainPanel->AttachChildUI(*MoveUIOwner);

		MainPanel->SetMouseFocusEvent([this]()->void
			{
				bOnMainPanel = true;
			});
	}
	void OnEditState() override
	{
		CUI* MainPanel = GetTileMapEditContext().MainPanel;
		CTileMap* TileMap = GetTileMapEditContext().TileMap;
		CTileMapper& TileMapper = GetTileMapEditContext().TileMapper;
		CTileFocus* TileFocus = GetTileMapEditContext().TileFocus;
		CTileHandler* TileHandler = GetTileMapEditContext().TileHandler;

		if (bOnMainPanel == false)
			return;

		const Vector2& MouseWorldPosition = GetMouseWorldPosition();
		CTile* FocusTile = TileMap->GetTileByPosition(MouseWorldPosition);
		if (FocusTile == nullptr)
			return;

		TileFocus->SetFocusTile(FocusTile, FocusTile->GetSpriteRenderComponent()->GetLayer() + 1);

		if (LHold())
		{
			if (TileMapper.IsAlreadyMapping(*FocusTile) == false)
			{
				CStaticActor* Actor = ActorGenerator.GenerateStaticActor(ImageImporter, FocusTile->GetTransform()->GetFinalPosition2D());
				if (Actor)
					TileMapper.Map(*FocusTile, *Actor);
			}
			else
			{
				if (TileHandler->IsExist(*FocusTile) == false)
					TileHandler->HandleTile(*FocusTile, FocusTile->GetSpriteRenderComponent()->GetLayer() + 2);
			}
		}
		else if (LReleased())
		{
			if (TileHandler->IsEmpty())
				return;
			Vector3 CenterPosition = TileHandler->GetCenterPosition();
			MoveUIOwner->Activate(true);
			MoveUIOwner->GetTransform()->SetPosition(CenterPosition);
		}
		else if (RHold())
		{
			TileHandler->ClearHandledTiles();
			if (TileMapper.IsAlreadyMapping(*FocusTile))
				TileMapper.UnMap(*FocusTile);
			MoveUIOwner->Activate(false);
		}

		bOnMainPanel = false;
	}

	void ExitEditState() override
	{
		CTileMap* TileMap = GetTileMapEditContext().TileMap;
		CUI* MainPanel = GetTileMapEditContext().MainPanel;
		MainPanel->SetMouseFocusEvent(nullptr);

		MoveUIOwner->Activate(false);
		MainPanel->DetachChildUI(*MoveUIOwner);
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
	CActorGenerator ActorGenerator;
	bool bOpenWindowDialog = false;

	CUI* MoveUIOwner = nullptr;

	bool bOnMainPanel = false;

};

