#include "pch.h"
#include "Character.h"

#include "GameCore.h"
void CCharacter::AddPart(const std::string& InPartName, CPart* InPart)
{
    if (Parts.contains(InPartName))
        return;
    if (InPart)
        Parts.emplace(InPartName, InPart);
    else
    {
        CPart* NewPart = GetWorld()->SpawnActor<CPart>(GetOwnerActor());
        Parts.emplace(InPartName, NewPart);
    }
}