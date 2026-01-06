#include "pch.h"
#include "DynamicActor.h"

CDynamicActor::CDynamicActor()
	: CStaticActor()
{
	AddComponent<CAnimator>();
}