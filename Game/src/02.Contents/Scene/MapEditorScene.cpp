#include "pch.h"
#include "MapEditorScene.h"

#include "GameCore.h"
#include "00.App/WindowManager.h"

void CMapEditorScene::BeginPlay()
{
	CScene::BeginPlay();
	GetFader()->FadeIn(0.0f);

	ActorGenerator = GetWorld()->SpawnActor<CActorGenerator>(this);
	TileManager = GetWorld()->SpawnActor<CTileManager>(this);
}

void CMapEditorScene::Update(float InDeltaTime)
{
	CScene::Update(InDeltaTime);

	if (bOpenWindowDialog)
	{
		ActorGenerator->SetGeneratedActorImagePathByWindowManager(CWindowManager::GetInst());
		bOpenWindowDialog = false;
	}

	if (bLayTiles)
	{
		TileManager->LayTiles(TileWidth, TileHeight, TileMapRow, TileMapCol);
		EditMode = EEditMode::Tile;
		bLayTiles = false;
	}

	const int32_t MouseX = InteractionComponent->GetMouseInteracter()->GetCurrentMouseX();
	const int32_t MouseY = InteractionComponent->GetMouseInteracter()->GetCurrentMouseY();
	Vector2 World2DPosition = Vector2(float(MouseX), float(MouseY));

	switch (EditMode)
	{
	case EEditMode::Free:
		if (LClicked())
			ActorGenerator->GenerateStaticActor(World2DPosition);
		break;
	case EEditMode::Tile:
	{
		CTile* ProximateTile = TileManager->GetProximateTile(World2DPosition);
		if (ProximateTile == nullptr)
			break;

		Vector2 ProximateTileWorldPosition = ProximateTile->GetTransform()->Get2DFinalPosition();

		if (ProximateTile && LHold())
		{
			const CStaticActor* TilePutOnActor = ProximateTile->GetPutOnActor();
			if (TilePutOnActor)
			{
				const std::wstring& TilePutOnActorImagePath = TilePutOnActor->GetSpriteRenderComponent()->GetImagePath();
				const std::wstring& GeneratedActorImagePath = ActorGenerator->GetGeneratedActorImagePath();
				if (TilePutOnActorImagePath == GeneratedActorImagePath)
					break;
			}
			CStaticActor* GeneratedActor = ActorGenerator->GenerateStaticActor(ProximateTileWorldPosition);
			ProximateTile->SetPutOnActor(GeneratedActor);
		}

		if (ProximateTile && RHold())
		{
			ActorGenerator->EraseActor(ProximateTileWorldPosition);
			ProximateTile->SetPutOnActor(nullptr);
		}
	}
	break;
	default:
		break;
	}

}

void CMapEditorScene::CaptureSnapShot()
{
	CScene::CaptureSnapShot();

	if (ImGui::Button("OpenWindowDialog"))
		bOpenWindowDialog = true;

	ImGui::InputScalar("TileWidth", ImGuiDataType_U64, &TileWidth);
	ImGui::InputScalar("TileHeight", ImGuiDataType_U64, &TileHeight);
	ImGui::InputScalar("TileMapRow", ImGuiDataType_U64, &TileMapRow);
	ImGui::InputScalar("TileMapCol", ImGuiDataType_U64, &TileMapCol);
	if (ImGui::Button("LayTiles"))
		bLayTiles = true;

	/*ImGui::RadioButton("CenterMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::Center));
	ImGui::SameLine();
	ImGui::RadioButton("DownMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::Down));
	ImGui::SameLine();
	ImGui::RadioButton("UpMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::Up));
	ImGui::RadioButton("LeftMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::Left));
	ImGui::SameLine();
	ImGui::RadioButton("RightMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::Right));
	ImGui::SameLine();
	ImGui::RadioButton("RightBottomMode", (int*)&TilePutMode, static_cast<int>(ETilePutMode::RightBottom));*/

	/*ImGui::BeginChild("TileList", ImVec2(0, 0), true);
	for (auto& Pair : LoadedImagePaths)
	{
		const std::string& ImageName = Pair.first;
		const std::wstring& ImagePath = Pair.second;

		bool bSelected = (CurrentActorImagePath == ImagePath);

		if (ImGui::Selectable(ImageName.c_str(), bSelected))
		{
			CurrentActorImagePath = ImagePath;
			ImGui::SetItemDefaultFocus();
		}
	}
	ImGui::EndChild();*/
}