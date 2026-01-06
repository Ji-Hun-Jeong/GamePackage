#pragma once
#include "../Component.h"
#include "Animator.h"

class CAnimatorController : public CComponent
{
	GENERATE_OBJECT(CAnimatorController)
public:
	CAnimatorController() = default;
	~CAnimatorController() = default;

public:
	void AddAnimator(CAnimator& InAnimator)
	{
		Animators.push_back(&InAnimator);
	}
	void PlayAnimation()
	{

	}

private:
	std::vector<CAnimator*> Animators;

};

