#pragma once
#include "Component.h"

class CRigidBody : public CComponent
{
	GENERATE_OBJECT(CRigidBody)
public:
	CRigidBody() = default;
	~CRigidBody() = default;

public:
	void Update(class CTransform& InTransform, float InDeltaTime);

	void SetForce(const Vector2& InForce) { Force = InForce; }
	void SetAccel(const Vector2& InAccel) { Accel = InAccel; }
	void SetVelocity(const Vector2& InVelocity) { Velocity = InVelocity; }
	void SetCoeff(const Vector2& InCoeff) { Coeff = InCoeff; }
	void SetMass(float InMass) { Mass = InMass; }

	void AddForce(const Vector2& InForce) { Force += InForce; }
	void AddAccel(const Vector2& InAccel) { Accel += InAccel; }
	void AddVelocity(const Vector2& InVelocity) { Velocity += InVelocity; }
	void AddCoeff(const Vector2& InCoeff) { Coeff += InCoeff; }
	void AddMass(float InMass) { Mass += InMass; }

	const Vector2& GetForce() const { return Force; }
	const Vector2& GetAccel() const { return Accel; }
	const Vector2& GetVelocity() const { return Velocity; }
	const Vector2& GetCoeff() const { return Coeff; }
	float		   GetMass() const { return Mass; }

	void SetMaxSpeed(const Vector2& _MaxSpeed) { MaxSpeed = _MaxSpeed; }

private:
	Vector2 Force;
	Vector2 Accel;
	Vector2 Velocity;
	Vector2 Coeff = Vector2(0.2f, 0.0f);

	Vector2 MaxSpeed = Vector2(0.35f);

	float Mass = 1.0f;
	float Gravity = 0.98f;

};

