#include "pch.h"
#include "Component.h"
#include "01.Base/Object/Actor.h"

CComponent::~CComponent()
{

}

void CComponent::SetOwner(CActor* InOwnerActor)
{
	InOwnerActor->Attach(this);
}

void CComponent::Destroy()
{
	// 얘 단독으로 지워도 다음 프레임에 지워져야 하기 때문에
	GetWorld()->PushWorldSynchronizeEvent([this]()->void
		{
			// Todo: World에게 요청
			EndPlay();
		});
}
