#include "pch.h"
#include "MousePointer.h"
#include "WindowIOManager.h"

void CMousePointer::SetMouseImageFromDialog(EKeyType InKeyType, CWindowIOManager& InWindowIOManager)
{
	if (InKeyType != EKeyType::LButton)
		return;

	if (InWindowIOManager.TryOpenFileDialog())
	{
		const std::wstring& ImagePath = InWindowIOManager.GetOpenFilePath();
		GetRenderComponent()->SetDiffuseImage(ImagePath);
	}
	else
		GetRenderComponent()->ResetImage();
}