#pragma once
#include "01.Base/Actor/Component/Component.h"
#include "01.Base/Actor/Actor.h"

#include "Common/Json.h"

class CCharacter : public CComponent
{
	GENERATE_OBJECT(CCharacter)
public:
	CCharacter() = default;
	~CCharacter() = default;

public:
    void ReadCharacterJson(const std::string& InFilePath)
    {
        
    }
	
private:
	std::unordered_map<std::string, CActor*> Parts;

};

