#include "pch.h"
#include "RigidBody.h"

#include "Transform.h"

void CRigidBody::Update(CTransform& InTransform, float InDeltaTime)
{
	Force.y -= Gravity * Mass;

	Accel = Force / Mass;	// 가속도

	Velocity += Accel * InDeltaTime;

	// 반작용
	Vector2 NormVelocity = Velocity;
	NormVelocity.Normalize();
	if (0 < Velocity.Length())
		Velocity -= NormVelocity * Coeff * InDeltaTime;

	const float AbsVelocityX = std::abs(Velocity.x);
	const float AbsVelocityY = std::abs(Velocity.y);
	if (AbsVelocityX < 0.0001f)
		Velocity.x = 0.0f;

	if (AbsVelocityY < 0.0001f)
		Velocity.y = 0.0f;

	if (MaxSpeed.x < AbsVelocityX)
		Velocity.x = Velocity.x < 0 ? -MaxSpeed.x : MaxSpeed.x;
	if (MaxSpeed.y < AbsVelocityY)
		Velocity.y = Velocity.y < 0 ? -MaxSpeed.y : MaxSpeed.y;

	Force = Vector2(0.0f);

	InTransform.Move(Vector3(Velocity.x, Velocity.y, 0.0f));
}
