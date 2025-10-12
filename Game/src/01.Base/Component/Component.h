#pragma once

#define GenerateComponent() \
public:\
	static uint32_t GetStaticType()\
	{\
		static uint32_t ComponentType = sComponentType++;\
		return ComponentType;\
	}\
	uint32_t GetType() override	{return GetStaticType();}

class CComponent
{
public:
	CComponent() = default;
	virtual ~CComponent() = 0 {}

public:
	virtual uint32_t GetType() = 0;
	
protected:
	static uint32_t sComponentType;

};

