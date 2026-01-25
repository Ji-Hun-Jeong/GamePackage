#pragma once
#include "01.Base/Actor/StaticActor.h"

class CEffector : public CStaticActor
{
	friend class CEffectPool;
	GENERATE_OBJECT(CEffector)
public:
	CEffector()
	{
		AddComponent<CAnimator>();
	}
	~CEffector() = default;

public:
	void Update(float InDeltaTime) override
	{
		CStaticActor::Update(InDeltaTime);

		if (Animator->GetCurrentAnimation()->IsFinish())
			if (FinishAction)
				FinishAction(Index);
	}
	void Reset()
	{
		Transform->Reset();
		Animator->Reset();
		SpriteRenderComponent->Reset();
	}

private:
	std::function<void(size_t)> FinishAction;
	size_t Index = 0;

};
class CEffectPool
{
	SINGLE(CEffectPool)
public:
	~CEffectPool() = default;

public:
	void Initalize(class CWorld& InWorld, size_t InPoolSize = 50);
	void TakePool(size_t InPoolSize);
	CEffector* GetEffector() 
	{
		if (Indices.empty())
			return nullptr;

		size_t Index = Indices.front();
		Indices.pop();

		CEffector* Effector = Effectors[Index];
		Effector->Activate(true);

		return Effector;
	}

private:
	void ReturnEffector(size_t InIndex)
	{
		Effectors[InIndex]->Reset();
		Effectors[InIndex]->Activate(false);
		
		Indices.emplace(InIndex);
	}
private:
	class CWorld* World = nullptr;

	std::vector<CEffector*> Effectors;
	std::queue<size_t> Indices;
};

