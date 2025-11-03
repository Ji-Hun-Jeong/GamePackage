#include "pch.h"
#include "MousePointer.h"
#include "WindowIOManager.h"

void CMousePointer::SetMouseImageFromDialog(CWindowIOManager& InWindowIOManager)
{
	if (InWindowIOManager.TryOpenFileDialog())
	{
		const std::wstring& ImagePath = InWindowIOManager.GetOpenFilePath();
		RenderComponent->SetDiffuseImage(ImagePath);
	}
	else
		RenderComponent->SetDiffuseImage(L"");
}