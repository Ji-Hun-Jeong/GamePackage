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

	}
	void OnEditState(TEditContext& InEditContext) override
	{
		CTileMap* TileMap = InEditContext.TileMap;
		if (bLayTiles)
		{
			TileMap->DetachToPanel(*InEditContext.MainPanel);
			TileMap->LayTiles(TileWidth, TileHeight, TileMapRow, TileMapCol);

			TileMap->AttachToPanel(*InEditContext.MainPanel, [this, TileMap](CTile& InFocusTile)->void
				{
					TileFocus->SetFocusTile(&InFocusTile);
					if (LHold())
					{
						if (TileMapper.IsAlreadyMapping(InFocusTile) == false)
						{
							CStaticActor* Actor = ActorGenerator.GenerateStaticActor(ImageImporter, InFocusTile.GetTransform()->GetFinalPosition2D());
							if (Actor)
								TileMapper.Map(InFocusTile, *Actor);
						}
						else
						{
							if (TileHandler->IsExist(InFocusTile) == false)
								TileHandler->HandleTile(InFocusTile, TileFocus->GetSpriteRenderComponent()->GetLayer() + 1);
						}
					}
					else if (LReleased())
					{
						if (TileHandler->IsEmpty())
							return;
						Vector3 CenterPosition = TileHandler->GetCenterPosition();
						CTile* ProximateTile = TileMap->GetTileByPosition(CenterPosition);
						if (ProximateTile)
						{
							Vector3 Offset = CenterPosition - ProximateTile->GetTransform()->GetFinalPosition();
							AttachUIs(*ProximateTile, Offset, TileFocus->GetSpriteRenderComponent()->GetLayer() + 2);
						}
					}
					else if (RHold())
					{
						TileHandler->ClearHandledTiles();
						if (TileMapper.IsAlreadyMapping(InFocusTile))
						{
							CStaticActor& MappingActor = TileMapper.UnMap(InFocusTile);
							MappingActor.Destroy();
						}
						DetachUIs();
					}
				});
			bLayTiles = false;
		}
	}

	void ExitEditState(TEditContext& InEditContext) override
	{
		InEditContext.TileMap->DetachToPanel(*InEditContext.MainPanel);
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
		InMoveUI->SetRectUI(3);
		InMoveUI->GetTransform()->SetScale(Vector3(20.0f, 20.0f, 1.0f));
		MoveUIs.push_back(InMoveUI);
	}
	void AttachUIs(CUI& InOwnerUI, const Vector3& InOffset, uint32_t InLayer)
	{
		for (auto UI : MoveUIs)
		{
			UI->Activate(true);
			UI->GetSpriteRenderComponent()->SetLayer(InLayer);
			InOwnerUI.AttachChildUI(*UI);
		}
	}
	void DetachUIs()
	{
		for (auto UI : MoveUIs)
		{
			UI->Activate(false);
			if (UI->GetOwnerUI())
				UI->GetOwnerUI()->DetachChildUI(*UI);
		}
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

	std::vector<CUI*> MoveUIs;

};

