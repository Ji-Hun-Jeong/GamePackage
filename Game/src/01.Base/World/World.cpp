#include "pch.h"
#include "World.h"

class CIO : public CActor
{
	GenerateObject()
public:
	void Initalize() override
	{
		CActor::Initalize();

		SetRenderComponent();
		RenderComponent->SetImage(L"resources/image/Player/Alert/0.png");

		Transform->SetSpeed(0.001f);
	}

	void Update(float InDeltaTime) override
	{
		CActor::Update(InDeltaTime);
		Transform->Move(Vector3(-1.0f, 0.0f, 0.0f));
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
	CIO* co = SpawnActor<CIO>();
	co->GetRenderComponent()->SetImage(L"resources/image/Player/AttackEffect/0.png");
	co->GetTransform()->SetSpeed(0.0001f);
	SpawnActor<CIO>();
}
