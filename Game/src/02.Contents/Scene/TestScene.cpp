#include "pch.h"
#include "TestScene.h"
#include "Scene2.h"

#include "01.Base/World/World.h"
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
void CTestScene::Initalize()
{
	CScene::Initalize();

	GetWorld()->SpawnActor<CC>(this);
	GetWorld()->SpawnActor<CPlayer>(this);


}