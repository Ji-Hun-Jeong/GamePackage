#include "pch.h"
#include "Object.h"

#include "03.Utils/NumberGenerator/NumberGenerator.h"

class CReleaseNumber : public IObjectDestroy
{
public:
	CReleaseNumber(CNumberGenerator& InNumberGenerator)
		: NumberGenerator(InNumberGenerator)
	{}

private:
	void OnDestroy(CObject& InObject) override
	{
		NumberGenerator.ReleaseNumber(InObject.GetInstanceId());
	}

	CNumberGenerator& NumberGenerator;
};

CObject::CObject(const CObject& InOther)
	: InstanceId(0)
	, World(nullptr)
{
}

CObject::CObject(CObject&& InOther) noexcept
	: InstanceId(InOther.InstanceId)
	, ObjectDestroyEvents(std::move(InOther.ObjectDestroyEvents))
{
	InOther.InstanceId = 0;
}

void CObject::SetInstanceId(CNumberGenerator& InNumberGenerator)
{
	InstanceId = InNumberGenerator.GenerateNumber();

	RegistObjectDestroyEvent(std::make_unique<CReleaseNumber>(InNumberGenerator));
}

void CObject::SetOwner(CObject* InOwner)
{
	Owner = InOwner;
	if (Owner)
		Owner->Childs.push_back(std::unique_ptr<CObject>(this));
}

void CObject::SetWorld(CWorld* InWorld)
{
	World = InWorld;
}
