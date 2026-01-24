#include "pch.h"
#include "EffectPool.h"

#include "GameCore.h"

CEffectPool::CEffectPool()
{

}

void CEffectPool::Initalize(CWorld& InWorld, size_t InPoolSize)
{
	World = &InWorld;
	TakePool(InPoolSize);
}

void CEffectPool::TakePool(size_t InPoolSize)
{
	assert(World);

	Effectors.resize(InPoolSize);
	for (size_t i = 0; i < Effectors.size(); ++i)
	{
		CEffector* Effector = World->SpawnActor<CEffector>(nullptr);
		Effector->Activate(false);
		Effector->FinishAction = std::bind(&CEffectPool::ReturnEffector, this, std::placeholders::_1);
		Effector->Index = i;

		Effectors[i] = Effector;

		Indices.emplace(i);
	}
}
