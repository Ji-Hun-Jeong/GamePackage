#pragma once
#include "../Object.h"

class CComponent : public CObject
{
	friend class CActor;
public:
	CComponent()
		: OwnerActor(nullptr)
		, bDestroy(false)
	{}
	virtual ~CComponent();

public:
	virtual void Initalize() override {}
	void SetOwner(class CActor* InOwnerActor) { assert(InOwnerActor); OwnerActor = InOwnerActor; }
	class CActor* GetOwnerActor() const { return OwnerActor; }
	void Destroy() override;
	bool IsDestroy() const { return bDestroy; }
	virtual void Serialize(CSerializer& InSerializer) const override {}
	virtual void Deserialize(const CSerializer& InDeserializer) override {}

private:
	class CActor* OwnerActor;
	bool bDestroy;

};

