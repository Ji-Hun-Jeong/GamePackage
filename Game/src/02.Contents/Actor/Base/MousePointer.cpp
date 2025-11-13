#include "pch.h"
#include "MousePointer.h"
#include "WindowIOManager.h"

void CMousePointer::SetMouseImageFromDialog(CWindowIOManager& InWindowIOManager)
{
	if (InWindowIOManager.TryOpenFileDialog())
	{
		const std::wstring& ImagePath = InWindowIOManager.GetOpenFilePath();
		SpriteRenderComponent->SetDiffuseImage(ImagePath);
	}
	else
		SpriteRenderComponent->SetDiffuseImage(L"");
}