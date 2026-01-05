#include "pch.h"
#include "FreeEditState.h"

#include "GameCore.h"
void CFreeEditState::OnEditState()
{
	if (bCreateImage)
	{
		if (ImageImporter.IsExistCurrentImagePath())
		{
			CStaticActor* Object = GetWorld()->SpawnActor<CStaticActor>(this);
			Object->GetSpriteRenderComponent()->SetDiffuseImage(ImageImporter.GetCurrentImagePath());

			ObjectManager.AddObject(*Object);

			CurrentFocusObject = Object;
		}
		bCreateImage = false;
	}

	if (CurrentFocusObject)
		CurrentFocusObject->GetTransform()->SetPosition(Vector3(PosArray[0], PosArray[1], PosArray[2]));

	if (bOnMainPanel == false)
		return;
	bOnMainPanel = false;

	if (LClicked())
	{
		const Vector2& MouseWorldPosition = GetMouseWorldPosition();
		CStaticActor* Object = ObjectManager.GetObjectByPosition(MouseWorldPosition);
		if (Object)
		{
			CurrentFocusObject = Object;
		}
		else
			CurrentFocusObject = nullptr;
	}
}