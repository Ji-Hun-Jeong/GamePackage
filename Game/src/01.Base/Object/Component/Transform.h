#pragma once
#include "../Object.h"

class CTransform : public CObject
{
	GenerateObject()
public:
	CTransform()
		: Position(Vector3(0.0f))
		, Rotation(Vector3(0.0f))
		, Scale(Vector3(1.0f))
		, Speed(0.0f)
		, bVariationFlag(true)
	{}
	~CTransform() = default;

public:
	void SetSpeed(float InSpeed) { Speed = InSpeed; }
	void Move(const Vector3& InDirection)
	{
		if (Speed == 0.0f) 
			std::cout << "Speed Is Zero\n";
		Position += InDirection * Speed;
		bVariationFlag = true;
	}
	void CalculateModelMatrix()
	{
		if (bVariationFlag == false)
			return;

		ModelMatrix = Matrix::CreateScale(Scale)
			* Matrix::CreateRotationX(Rotation.x)
			* Matrix::CreateRotationY(Rotation.y)
			* Matrix::CreateRotationZ(Rotation.z)
			* Matrix::CreateTranslation(Position);

		ModelMatrix = ModelMatrix.Transpose();

		bVariationFlag = false;
	}
	const Matrix& GetModelMatrix() const { return ModelMatrix; }

private:
	Matrix ModelMatrix;
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;

	float Speed;

	bool bVariationFlag;
};

