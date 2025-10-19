#pragma once
#include "Component.h"

class CTransform : public CComponent
{
    GENERATE_OBJECT()
public:
    CTransform()
        : FinalPosition(Vector3(0.0f))
        , Position(Vector3(0.0f))
        , Rotation(Vector3(0.0f))
        , Scale(Vector3(1.0f))
        , OffsetScale(Vector3(1.0f))
        , Speed(0.0f)
        , bVariationFlag(true)
    {}
    ~CTransform() = default;

public:
    void Serialize(CSerializer& InSerializer) const override
    {
        CComponent::Serialize(InSerializer);
        CSerializer Data;
        Data["position"] = { {"x", Position.x}, {"y", Position.y} ,{"z", Position.z} };
        Data["rotation"] = { {"x", Rotation.x}, {"y", Rotation.y} ,{"z", Rotation.z} };
        Data["scale"] = { {"x", Scale.x}, {"y", Scale.y} ,{"z", Scale.z} };
        Data["speed"] = Speed;
        InSerializer["transform"] = Data;
    }
    // =================================================================
    // Getters
    // =================================================================
    const Vector3& GetFinalPosition() const { return FinalPosition; }
    const Vector3& GetPosition() const { return Position; }
    const Vector3& GetRotation() const { return Rotation; }
    const Vector3& GetScale() const { return Scale; }
    const Vector3& GetOffsetScale() const { return OffsetScale; }
    const Vector3& GetFinalScale() const { return FinalScale; }
    float GetSpeed() const { return Speed; }

    // =================================================================
    // Setters
    // =================================================================
    void SetFinalPosition(const Vector3& InFinalPosition)
    {
        FinalPosition = InFinalPosition;
    }
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
    void SetOffsetScale(const Vector3& InOffsetScale)
    {
        OffsetScale = InOffsetScale;
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
        FinalScale = Scale * OffsetScale;
        ModelMatrix = Matrix::CreateScale(FinalScale)
            * Matrix::CreateRotationX(Rotation.x)
            * Matrix::CreateRotationY(Rotation.y)
            * Matrix::CreateRotationZ(Rotation.z)
            * Matrix::CreateTranslation(FinalPosition);

        ModelMatrix = ModelMatrix.Transpose();
    }
    
    const Matrix& GetModelMatrix() const { return ModelMatrix; }
    void SetVariation(bool bVariation) { bVariationFlag = bVariation; }
    bool OnVariation() const { return bVariationFlag; }

private:
    Matrix ModelMatrix;
    Vector3 FinalPosition;
    Vector3 Position;
    Vector3 Rotation;
    Vector3 Scale;
    Vector3 OffsetScale;
    Vector3 FinalScale;

    float Speed;

    bool bVariationFlag;
};