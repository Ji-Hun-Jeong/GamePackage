#pragma once
#include "../Object.h"

class CComponent : public CObject
{
public:
	CComponent() = default;
	virtual ~CComponent() = 0 {}

};

