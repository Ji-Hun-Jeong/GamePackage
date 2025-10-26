#include "pch.h"
#include "MousePointer.h"
#include "WindowIOManager.h"

void CMousePointer::Initalize()
{
	CActor::Initalize();
	SetRenderComponent();
}

void CMousePointer::SetMouseImageFromDialog(CWindowIOManager& InWindowIOManager)
{
	if (InWindowIOManager.TryOpenFileDialog())
	{
		const std::wstring& ImagePath = InWindowIOManager.GetOpenFilePath();
		GetRenderComponent()->SetDiffuseImage(ImagePath);
	}
	else
		GetRenderComponent()->ResetImage();
}