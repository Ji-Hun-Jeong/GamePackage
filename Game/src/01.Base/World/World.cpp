#include "pch.h"
#include "World.h"
#include "02.Contents/Actor/Player/Player.h"

class CC : public CCharacter
{
	GenerateObject()
		DONTCOPY(CC)
public:
	CC() = default;
	~CC() = default;

public:
	void Initalize()
	{
		CCharacter::Initalize();

		GetRenderComponent()->SetImage(L"resources/image/Player/Alert/0.png");
		GetTransform()->SetSpeed(0.001f);
	}

	void Update(float InDeltaTime)
	{
		CCharacter::Update(InDeltaTime);

		GetTransform()->Move(Vector3(-1.0f, 0.0f, 0.0f));
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
	SpawnActor<CPlayer>();
	//SpawnActor<CC>();
}
