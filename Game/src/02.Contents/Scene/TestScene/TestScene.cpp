#include "pch.h"
#include "TestScene.h"
#include "01.Base/World/World.h"

class AActor : public CActor
{
	// CActor을(를) 통해 상속됨
	CObject* Clone() override
	{
		return nullptr;
	}
	void Update(float InDeltaTime) override
	{
		std::cout << "HI\n";
	}
};
TestScene::TestScene(CWorld& InWorld)
	: CScene("Test")
{
	InWorld.SpawnActor<AActor>(this);
}
