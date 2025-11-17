#include "pch.h"
#include "MousePointer.h"

#include "00.App/WindowManager.h"

void CMousePointer::SetMouseImageFromDialog(CWindowManager& InWindowManager)
{
	std::wstring ImagePath;
	if (InWindowManager.TryGetFilePathByDialog(&ImagePath))
		SpriteRenderComponent->SetDiffuseImage(ImagePath);
	else
		SpriteRenderComponent->SetDiffuseImage(L"");
}