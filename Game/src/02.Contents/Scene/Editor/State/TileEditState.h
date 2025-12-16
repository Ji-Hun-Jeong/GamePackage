#pragma once
#include "IEditState.h"
#include "02.Contents/Actor/Tile/TileHandler.h"

class CTileEditState : public IEditState
{
	GENERATE_OBJECT(CTileEditState)
public:
	CTileEditState();
	~CTileEditState() = default;

public:
	void EnterEditState(TEditContext& InEditContext) override
	{
		CTileMap* TileMap = InEditContext.TileMap;
		CUI* MainPanel = InEditContext.MainPanel;
		MainPanel->SetUILayer(0);

		MainPanel->SetMouseFocusEvent([this]()->void
			{
				bChooseTile = true;
			});

		MoveUIOwner->Activate(false);
		MainPanel->AttachChildUI(*MoveUIOwner);
	}
	void OnEditState(TEditContext& InEditContext) override
	{
		CTileMap* TileMap = InEditContext.TileMap;
		CUI* MainPanel = InEditContext.MainPanel;

		if (bLayTiles)
		{
			TileMap->LayTiles(TileWidth, TileHeight, TileMapRow, TileMapCol);
			bLayTiles = false;
		}

		if (bChooseTile == false)
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
		bChooseTile = false;
	}

	void ExitEditState(TEditContext& InEditContext) override
	{
		InEditContext.MainPanel->SetMouseFocusEvent(nullptr);

		MoveUIOwner->Activate(false);
		InEditContext.MainPanel->DetachChildUI(*MoveUIOwner);
	}
	void ToImGUI() override
	{
		if (ImGui::Button("LayTiles"))
			bLayTiles = true;
		ImGui::InputScalar("TileWidth", ImGuiDataType_U64, &TileWidth);
		ImGui::InputScalar("TileHeight", ImGuiDataType_U64, &TileHeight);
		ImGui::InputScalar("TileMapRow", ImGuiDataType_U64, &TileMapRow);
		ImGui::InputScalar("TileMapCol", ImGuiDataType_U64, &TileMapCol);

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

	CTileMapper TileMapper;
	CTileFocus* TileFocus = nullptr;
	CTileHandler* TileHandler = nullptr;

	size_t TileWidth = 90;
	size_t TileHeight = 60;
	size_t TileMapRow = 30;
	size_t TileMapCol = 30;
	bool bLayTiles = false;

	CUI* MoveUIOwner = nullptr;

	bool bChooseTile = false;
};

