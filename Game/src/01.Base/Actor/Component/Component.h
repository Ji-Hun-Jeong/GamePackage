#pragma once
#include "00.App/CoreSystem.h"

class CComponent : public CObject
{
	friend class CActor;
public:
	CComponent()
		: OwnerActor(nullptr)
	{}
	virtual ~CComponent();

public:
	class CActor* GetOwnerActor() const { return OwnerActor; }

	virtual void Serialize(CSerializer& InSerializer) const override {}
	virtual void Deserialize(const CSerializer& InDeserializer) override {}

private:
	class CActor* OwnerActor;

};

