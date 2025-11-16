#include "pch.h"
#include "Actor.h"

CActor::CActor()
	: Owner(nullptr)
	, bActive(true)
	, Transform(nullptr)
	, InteractionComponent(nullptr)
{
	Transform = AddComponent<CTransform>();
}