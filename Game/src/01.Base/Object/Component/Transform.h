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
    // =================================================================
    // Getters
    // =================================================================
    const Vector3& GetPosition() const { return Position; }
    const Vector3& GetRotation() const { return Rotation; }
    const Vector3& GetScale() const { return Scale; }
    float GetSpeed() const { return Speed; }

    // =================================================================
    // Setters
    // =================================================================
    void SetPosition(const Vector3& InPosition)
    {
        Position = InPosition;
        SetVariation(true);
    }
    void SetRotation(const Vector3& InRotation)
    {
        Rotation = InRotation;
        SetVariation(true);
    }
    void SetScale(const Vector3& InScale)
    {
        Scale = InScale;
        SetVariation(true);
    }
    void SetSpeed(float InSpeed) { Speed = InSpeed; }

    // =================================================================
    // Other Methods
    // =================================================================
    void Move(const Vector3& InDirection)
    {
        if (Speed == 0.0f)
            std::cout << "Speed Is Zero\n";
        Position += InDirection * Speed;
        SetVariation(true);
    }
    void CalculateModelMatrix()
    {
        ModelMatrix = Matrix::CreateScale(Scale)
            * Matrix::CreateRotationX(Rotation.x)
            * Matrix::CreateRotationY(Rotation.y)
            * Matrix::CreateRotationZ(Rotation.z)
            * Matrix::CreateTranslation(Position);

        ModelMatrix = ModelMatrix.Transpose();
    }
    const Matrix& GetModelMatrix() const { return ModelMatrix; }
    void SetVariation(bool bVariation) { bVariationFlag = bVariation; }
    bool OnVariation() const { return bVariationFlag; }

private:
    Matrix ModelMatrix;
    Vector3 Position;
    Vector3 Rotation;
    Vector3 Scale;

    float Speed;

    bool bVariationFlag;
};