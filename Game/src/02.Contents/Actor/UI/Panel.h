#pragma once
#include "01.Base/Object/UI.h"
#include <stack>

class CPanel : public CUI
{
	GENERATE_OBJECT() 
public:
	CPanel() = default;
	~CPanel() = default;

public:


private:
	std::stack<CUI*> UIStack;

};

