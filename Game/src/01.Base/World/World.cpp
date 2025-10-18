#include "pch.h"
#include "World.h"
#include "../Object/Camera.h"
#include "01.Base/Object/Character.h"
#include "02.Contents/Actor/Player/Player.h"

class CC : public CCharacter
{
	GENERATE_OBJECT()
	DONTCOPY(CC)
public:
	CC() = default;
	~CC() = default;

public:
	void Initalize()
	{
		CCharacter::Initalize();

		GetRenderComponent()->SetImage(L"resources/image/Map/MushmomStage/MushmomBackground.png");
		GetTransform()->SetSpeed(0.001f);
	}

	void Update(float InDeltaTime)
	{
		CCharacter::Update(InDeltaTime);
	}

};

CWorld::CWorld()
	: bFlagDestroyedWorldObject(false)
{

}

CWorld::~CWorld()
{

}

void CWorld::Start()
{
	SpawnActor<CC>();
	SpawnActor<CPlayer>();
	SpawnActor<CCamera>();
}
