#pragma once
#include "../Object.h"

class CComponent : public CObject
{
	friend class CActor;
public:
	CComponent()
		: OwnerActor(nullptr)
	{}
	virtual ~CComponent();

public:
	virtual void Initalize() override {}
	void SetOwner(class CActor* InOwnerActor) override;
	class CActor* GetOwnerActor() const { return OwnerActor; }

	void Destroy() override;
	virtual void Serialize(CSerializer& InSerializer) const override {}
	virtual void Deserialize(const CSerializer& InDeserializer) override {}

private:
	class CActor* OwnerActor;

};

