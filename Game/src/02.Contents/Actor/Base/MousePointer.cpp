#include "pch.h"
#include "MousePointer.h"

void CMousePointer::SetMouseImageFromDialog(CWindowManager& InWindowManager)
{
	if (InWindowManager.TryOpenFileDialog())
	{
		const std::wstring& ImagePath = InWindowManager.GetOpenFilePath();
		SpriteRenderComponent->SetDiffuseImage(ImagePath);
	}
	else
		SpriteRenderComponent->SetDiffuseImage(L"");
}