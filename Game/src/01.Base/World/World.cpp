#include "pch.h"
#include "World.h"

class CCO : public CObject
{
public:
	void BeginPlay() override
	{
		CObject::BeginPlay();
		std::cout << "CCO\n";
	}
	void Update(float InDeltaTime) override
	{
		CObject::Update(InDeltaTime);
	}
};

class CO : public CObject
{
public:
	void BeginPlay() override
	{
		CObject::BeginPlay();
		std::cout << "CO\n";
		auto p = GetWorld()->NewObject<CCO>(this);
	}
	void Update(float InDeltaTime) override
	{
		CObject::Update(InDeltaTime);
		Destroy();
	}
};

CWorld::CWorld()
{
	CO* co = NewObject<CO>();
}

CWorld::~CWorld()
{
	
}
