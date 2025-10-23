#include "pch.h"
#include "MousePointer.h"
#include "WindowIOManager.h"
#include "01.Base/World/World.h"

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