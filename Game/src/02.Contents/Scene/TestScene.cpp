#include "pch.h"
#include "TestScene.h"

void CTestScene::Initalize()
{
	CScene::Initalize();
	CObject* Object = GetWorld()->Deserialize("resources/data/ui/test1.json", this);
	GetWorld()->AppearActor((CActor*)Object);
}
