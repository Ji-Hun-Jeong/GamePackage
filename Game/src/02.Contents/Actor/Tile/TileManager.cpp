#include "pch.h"
#include "TileManager.h"
#include "GameCore.h"

#include "02.Contents/Actor/Edit/ActorGenerator.h"
#include "04.Renderer/ImGuiManager.h"

void CTileManager::LayTiles(size_t InWidth, size_t InHeight, size_t InRow, size_t InCol)
{
	for (CTile* Tile : Tiles)
		Tile->Destroy();
	Tiles.clear();

	TileWidth = InWidth;
	TileHeight = InHeight;
	TileMapRow = InRow;
	TileMapCol = InCol;

	float FirstX = (-(InCol - 1.0f) / 2.0f) * InWidth;
	float FirstY = ((InRow - 1.0f) / 2.0f) * InHeight;
	float Z = 1.0f;
	Vector3 FirstPosition = Vector3(FirstX, FirstY, Z);
	Vector3 Scale = Vector3(float(InWidth), float(InHeight), 1.0f);

	Tiles.resize(InRow * InCol, nullptr);
	for (size_t i = 0; i < InRow; ++i)
	{
		for (size_t j = 0; j < InCol; ++j)
		{
			CTile*& Tile = Tiles[i * InCol + j];
			Tile = GetWorld()->SpawnActor<CTile>();
			Vector3 Position = FirstPosition + Vector3(float(InWidth * j), -float(InHeight * i), 0.0f);
			Tile->GetTransform()->SetPosition(Position);
			Tile->GetTransform()->SetScale(Scale);
		}
	}
}

void CTileManager::LayTiles(CActorGenerator& InActorGenerator, size_t InWidth, size_t InHeight, size_t InRow, size_t InCol)
{
	InActorGenerator.ClearActor();
	LayTiles(InWidth, InHeight, InRow, InCol);
}

void CTileManager::PutOnActorToProximateTile(CActorGenerator& InActorGenerator, const Vector2& InWorld2DPosition)
{
	CTile* ProximateTile = GetProximateTile(InWorld2DPosition);
	if (ProximateTile == nullptr)
		return;

	CStaticActor* PutOnActor = ProximateTile->GetPutOnActor();
	if (PutOnActor)
	{
		InActorGenerator.EraseActor(*ProximateTile->GetPutOnActor());
		ProximateTile->SetPutOnActor(nullptr);
	}

	Vector2 ProximateTileWorldPosition = ProximateTile->GetTransform()->Get2DFinalPosition();
	const CStaticActor* TilePutOnActor = ProximateTile->GetPutOnActor();
	if (TilePutOnActor)
	{
		const std::wstring& TilePutOnActorImagePath = TilePutOnActor->GetSpriteRenderComponent()->GetImagePath();
		const std::wstring& GeneratedActorImagePath = InActorGenerator.GetGeneratedActorImagePath();
		if (TilePutOnActorImagePath == GeneratedActorImagePath)
			return;
	}
	CStaticActor* GeneratedActor = InActorGenerator.GenerateStaticActor(ProximateTileWorldPosition);
	ProximateTile->SetPutOnActor(GeneratedActor);
}

void CTileManager::PutOffActorToProximateTile(CActorGenerator& InActorGenerator, const Vector2& InWorld2DPosition)
{
	CTile* ProximateTile = GetProximateTile(InWorld2DPosition);
	if (ProximateTile == nullptr)
		return;

	InActorGenerator.EraseActor(*ProximateTile->GetPutOnActor());
	ProximateTile->SetPutOnActor(nullptr);
}

void CTileManager::SnapActorOnProximateTile(const Vector2& InWorld2DPosition)
{
	CTile* ProximateTile = GetProximateTile(InWorld2DPosition);
	if (ProximateTile == nullptr)
		return;
	CStaticActor* PutOnActor = ProximateTile->GetPutOnActor();
	if (PutOnActor == nullptr)
		return;

	const Vector2 ProximateTileWorldPosition = ProximateTile->GetTransform()->Get2DFinalPosition();
	const Vector2 ProximateTileScale = ProximateTile->GetTransform()->Get2DScale();

	float MinDist = ProximateTileScale.x < ProximateTileScale.y ? ProximateTileScale.y : ProximateTileScale.x;

	const float Top = ProximateTileWorldPosition.y + ProximateTileScale.y / 2.0f;
	const float Bottom = ProximateTileWorldPosition.y - ProximateTileScale.y / 2.0f;
	const float Left = ProximateTileWorldPosition.x - ProximateTileScale.x / 2.0f;
	const float Right = ProximateTileWorldPosition.x + ProximateTileScale.x / 2.0f;

	const float TopSnapDist = (Vector2(ProximateTileWorldPosition.x, Top) - InWorld2DPosition).Length();
	const float BottomSnapDist = (Vector2(ProximateTileWorldPosition.x, Bottom) - InWorld2DPosition).Length();
	const float LeftSnapDist = (Vector2(Left, ProximateTileWorldPosition.y) - InWorld2DPosition).Length();
	const float RightSnapDist = (Vector2(Right, ProximateTileWorldPosition.y) - InWorld2DPosition).Length();
	const float CenterDist = (ProximateTileWorldPosition - InWorld2DPosition).Length();
	const float LeftTopDist = (Vector2(Left, Top) - InWorld2DPosition).Length();
	const float LeftBottomDist = (Vector2(Left, Bottom) - InWorld2DPosition).Length();
	const float RightTopDist = (Vector2(Right, Top) - InWorld2DPosition).Length();
	const float RightBottomDist = (Vector2(Right, Bottom) - InWorld2DPosition).Length();

	MinDist = std::min<float>(MinDist, TopSnapDist);
	MinDist = std::min<float>(MinDist, BottomSnapDist);
	MinDist = std::min<float>(MinDist, LeftSnapDist);
	MinDist = std::min<float>(MinDist, RightSnapDist);
	MinDist = std::min<float>(MinDist, CenterDist);
	MinDist = std::min<float>(MinDist, LeftTopDist);
	MinDist = std::min<float>(MinDist, LeftBottomDist);
	MinDist = std::min<float>(MinDist, RightTopDist);
	MinDist = std::min<float>(MinDist, RightBottomDist);

	const Vector2& ImageScale = PutOnActor->GetSpriteRenderComponent()->GetImageScale();
	Vector2 OffsetScale = ProximateTileScale - ImageScale;

	if (MinDist == TopSnapDist)
		PutOnActor->GetTransform()->SetPosition(Vector3(ProximateTileWorldPosition.x, ProximateTileWorldPosition.y + OffsetScale.y / 2.0f, 0.0f));
	else if (MinDist == BottomSnapDist)
		PutOnActor->GetTransform()->SetPosition(Vector3(ProximateTileWorldPosition.x, ProximateTileWorldPosition.y - OffsetScale.y / 2.0f, 0.0f));
	else if (MinDist == LeftSnapDist)
		PutOnActor->GetTransform()->SetPosition(Vector3(ProximateTileWorldPosition.x - OffsetScale.x / 2.0f, ProximateTileWorldPosition.y, 0.0f));
	else if (MinDist == RightSnapDist)
		PutOnActor->GetTransform()->SetPosition(Vector3(ProximateTileWorldPosition.x + OffsetScale.x / 2.0f, ProximateTileWorldPosition.y, 0.0f));
	else if(MinDist == CenterDist)
		PutOnActor->GetTransform()->SetPosition(Vector3(ProximateTileWorldPosition.x, ProximateTileWorldPosition.y, 0.0f));
	else if (MinDist == LeftTopDist)
		PutOnActor->GetTransform()->SetPosition(Vector3(Left, ProximateTileWorldPosition.y + OffsetScale.y / 2.0f, 0.0f));
	else if (MinDist == LeftBottomDist)
		PutOnActor->GetTransform()->SetPosition(Vector3(Left, ProximateTileWorldPosition.y - OffsetScale.y / 2.0f, 0.0f));
	else if (MinDist == RightTopDist)
		PutOnActor->GetTransform()->SetPosition(Vector3(Right, ProximateTileWorldPosition.y + OffsetScale.y / 2.0f, 0.0f));
	else if (MinDist == RightBottomDist)
		PutOnActor->GetTransform()->SetPosition(Vector3(Right, ProximateTileWorldPosition.y - OffsetScale.y / 2.0f, 0.0f));
}
