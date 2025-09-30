#include "pch.h"
#include "Object.h"

#include "../World/World.h"

#include "03.Utils/NumberGenerator/NumberGenerator.h"

CObject::CObject(const CObject& InOther)
	: InstanceId(0)
	, Owner(nullptr)
	, World(nullptr)
{
}

CObject::CObject(CObject&& InOther) noexcept
	: InstanceId(InOther.InstanceId)
	, ObjectDestroyEvents(std::move(InOther.ObjectDestroyEvents))
	, Owner(InOther.Owner)
	, World(InOther.World)
{
	InOther.InstanceId = 0;
}

