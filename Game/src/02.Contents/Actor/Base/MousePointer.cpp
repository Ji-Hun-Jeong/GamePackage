#include "pch.h"
#include "MousePointer.h"

void CMousePointer::SetMouseImageFromDialog(CWindowManager& InWindowManager)
{
	std::wstring ImagePath;
	if (InWindowManager.TryGetFilePathByDialog(&ImagePath))
		SpriteRenderComponent->SetDiffuseImage(ImagePath);
	else
		SpriteRenderComponent->SetDiffuseImage(L"");
}