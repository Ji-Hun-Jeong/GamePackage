#include "pch.h"
#include "MousePointer.h"
#include "WindowIOManager.h"
#include "01.Base/World/World.h"

void CMousePointer::Initalize()
{
	CActor::Initalize();
	SetRenderComponent();
	MousePositionComponent = GetWorld()->NewObject<CMousePositionComponent>(this);
}
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