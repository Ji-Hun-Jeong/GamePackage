#include "pch.h"
#include "ColliderEditState.h"

#include "GameCore.h"
void CColliderEditState::BeginPlay()
{
	CEditState::BeginPlay();

	GroundManager = GetWorld()->SpawnActor<CGroundManager>(this);
}
