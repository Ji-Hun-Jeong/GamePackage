#include "pch.h"
#include "TileHandler.h"
#include "GameCore.h"

#include "02.Contents/Actor/Manager/GroundManager.h"

void CTileHandler::HandleTile(CTile& InHandledTile, uint32_t InLayer)
{
	auto Iter = TileMarkerMap.find(&InHandledTile);
	if (Iter != TileMarkerMap.end())
		return;

	CStaticActor* TileMarker = GetWorld()->SpawnActor<CStaticActor>(this);
	TileMarker->GetTransform()->SetPosition(InHandledTile.GetTransform()->GetPosition());
	TileMarker->GetTransform()->SetScale(InHandledTile.GetTransform()->GetScale());
	TileMarker->SetLineActor();
	TileMarker->GetSpriteRenderComponent()->SetColor(Vector3(1.0f, 0.0f, 0.0f), 1.0f);
	TileMarker->GetSpriteRenderComponent()->SetLayer(InLayer);

	TileMarkerMap.emplace(&InHandledTile, TileMarker);
}

Vector3 CTileHandler::GetCenterPosition() const
{
	Vector3 CenterPosition;
	for (auto& Pair : TileMarkerMap)
		CenterPosition += Pair.first->GetTransform()->GetPosition();
	return CenterPosition / float(TileMarkerMap.size());
}

//void CTileHandler::SetGroundByHandledTiles(const CTileMap& InTileMap, CGroundManager& InGroundManager)
//{
//	std::vector<std::pair<size_t, CTile*>> TileKeyMap;
//	TileKeyMap.reserve(TileMarkerMap.size());
//	for (auto& Pair : TileMarkerMap)
//	{
//		size_t TileIndex = InTileMap.GetTileIndex(*Pair.first);
//		if (TileIndex == CTileMap::TileNone)
//			assert(0);
//		TileKeyMap.emplace_back(TileIndex, Pair.first);
//	}
//
//	std::sort(TileKeyMap.begin(), TileKeyMap.end(), [](const std::pair<size_t, CTile*>& InA, const std::pair<size_t, CTile*>& InB)->bool
//		{
//			return InA.first < InB.first; 
//		});
//
//	size_t TileMapRow = InTileMap.GetTileMapRow();
//	size_t TileMapCol = InTileMap.GetTileMapCol();
//
//	bool bSquare = true;
//	size_t PrevColCount = 0;
//	size_t ColCount = 0;
//	for (size_t i = 0; i < TileKeyMap.size(); ++i)
//	{
//		ColCount += 1;
//		if (TileKeyMap.size() - 1 == i)
//		{
//			if (PrevColCount == 0)
//				PrevColCount = ColCount;
//			if (PrevColCount != ColCount)
//				bSquare = false;
//			break;
//		}
//
//		size_t CurColIndex = TileKeyMap[i].first;
//		size_t NextColIndex = TileKeyMap[i + 1].first;
//
//		// 한번 끊겼다 == 다음 행으로 넘어갈 수도 있다.
//		if (CurColIndex + 1 < NextColIndex)
//		{
//			size_t NextRowFirstIndex = TileKeyMap[i - ColCount + 1].first + TileMapCol;
//
//			if (NextRowFirstIndex == NextColIndex)
//			{
//				if (PrevColCount == 0)
//					PrevColCount = ColCount;
//				if (PrevColCount != ColCount)
//				{
//					bSquare = false;
//					break;
//				}
//			}
//			else
//			{
//				bSquare = false;
//				break;
//			}
//			ColCount = 0;
//		}
//	}
//
//	if (bSquare)
//	{
//		size_t Row = TileMarkerMap.size() / PrevColCount;
//		size_t Col = PrevColCount;
//		size_t TileWidth = InTileMap.GetTileWidth();
//		size_t TileHeight = InTileMap.GetTileHeight();
//
//		Vector3 CenterPosition = GetCenterPosition();
//		size_t ColliderWidth = TileWidth * Col;
//		size_t ColliderHeight = TileHeight * Row;
//
//		InGroundManager.AddGroundCollider(Vector3(CenterPosition.x, CenterPosition.y, 1.0f), Vector3(ColliderWidth, ColliderHeight, 1.0f));
//	}
//	else
//	{
//		for (auto& Pair : TileMarkerMap)
//		{
//			const Vector3& TileFinalPosition = Pair.first->GetTransform()->GetFinalPosition();
//			const Vector3& TileScale = Pair.first->GetTransform()->GetScale();
//
//			InGroundManager.AddGroundCollider(TileFinalPosition, TileScale);
//		}
//	}
//
//	ClearHandledTiles();
//}

void CTileHandler::SetGroundByHandledTiles(const CTileMap& InTileMap, CGroundManager& InGroundManager)
{
    if (TileMarkerMap.empty())
        return;

    size_t TileMapCol = InTileMap.GetTileMapCol(); // 전체 맵의 열 개수 (인덱스 변환용)

    // 1. Min/Max 초기화 (반대로 설정해둠)
    size_t MinRow = SIZE_MAX, MaxRow = 0;
    size_t MinCol = SIZE_MAX, MaxCol = 0;

    // 2. 모든 선택된 타일을 순회하며 Min/Max 갱신 (정렬 필요 없음!)
    for (auto& Pair : TileMarkerMap)
    {
        size_t TileIndex = InTileMap.GetTileIndex(*Pair.first);
        if (TileIndex == CTileMap::TileNone) assert(0);

        // 인덱스를 (Row, Col) 좌표로 변환
        size_t CurRow = TileIndex / TileMapCol;
        size_t CurCol = TileIndex % TileMapCol;

        // Min, Max 갱신
        if (CurRow < MinRow) MinRow = CurRow;
        if (CurRow > MaxRow) MaxRow = CurRow;
        if (CurCol < MinCol) MinCol = CurCol;
        if (CurCol > MaxCol) MaxCol = CurCol;
    }

    // 3. 사각형 판별 (수학적 검증)
    // 가로 길이 = (최대 열 - 최소 열) + 1
    // 세로 길이 = (최대 행 - 최소 행) + 1
    size_t RectWidthCount = (MaxCol - MinCol) + 1;
    size_t RectHeightCount = (MaxRow - MinRow) + 1;
    size_t ExpectedTileCount = RectWidthCount * RectHeightCount;

    // 실제 타일 개수
    size_t ActualTileCount = TileMarkerMap.size();

    bool bSquare = (ExpectedTileCount == ActualTileCount);

    // 4. 결과 적용
    if (bSquare)
    {
        // 합쳐진 사각형 생성
        size_t TileWidth = InTileMap.GetTileWidth();
        size_t TileHeight = InTileMap.GetTileHeight();

        // 픽셀 단위 크기 계산
        float TotalWidth = (float)(RectWidthCount * TileWidth);
        float TotalHeight = (float)(RectHeightCount * TileHeight);

        // 중심점 계산: Min 위치에서 반만큼 이동
        // 주의: 타일맵의 월드 좌표계 기준에 따라 시작점 계산이 다를 수 있음.
        // 여기서는 MinRow/MinCol 타일의 좌표를 기준으로 계산하는 방식 예시

        // 간단하게는 기존 GetCenterPosition() 활용 (이미 무게중심일 테니)
        Vector3 CenterPosition = GetCenterPosition();

        InGroundManager.AddGround(
            Vector3(CenterPosition.x, CenterPosition.y, 1.0f),
            Vector3(TotalWidth, TotalHeight, 1.0f)
        );
    }
    else
    {
        // 낱개 생성
        for (auto& Pair : TileMarkerMap)
        {
            const Vector3& TileFinalPosition = Pair.first->GetTransform()->GetPosition();
            const Vector3& TileScale = Pair.first->GetTransform()->GetScale(); // 보통 타일 크기

            InGroundManager.AddGround(TileFinalPosition, TileScale);
        }
    }

    ClearHandledTiles();
}