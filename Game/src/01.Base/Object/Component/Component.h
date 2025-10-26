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
	void ResetOwner(class CActor* InOwnerActor) override;
	void SetOwner(class CActor* InOwnerActor) override;
	class CActor* GetOwner() const override { return OwnerActor; }

	void Destroy() override final;
	virtual void Serialize(CSerializer& InSerializer) const override {}
	virtual void Deserialize(const CSerializer& InDeserializer) override {}

private:
	class CActor* OwnerActor;

};

