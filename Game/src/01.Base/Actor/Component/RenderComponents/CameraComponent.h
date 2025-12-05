#pragma once
#include "RenderComponent.h"



class CCameraComponent : public CRenderComponent
{
	GENERATE_OBJECT(CCameraComponent)
public:
	CCameraComponent();
	~CCameraComponent() = default;

public:
	void Render(CSpriteRenderer& InRenderer, const Vector3& InPosition, const Vector3& InRotation, const Vector3& InScale) override;

};

