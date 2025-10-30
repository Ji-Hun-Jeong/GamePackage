#pragma once
#include "Component.h"

class CTransform : public CComponent
{
    GENERATE_OBJECT(CTransform)
public:
    CTransform()
        : FinalPosition(Vector3(0.0f))
        , Position(Vector3(0.0f))
        , Rotation(Vector3(0.0f))
        , Scale(Vector3(1.0f))
        , Speed(0.0f)
        , bVariationFlag(true)
    {}
    ~CTransform() = default;

public:
    void Serialize(CSerializer& InSerializer) const override
    {
        CComponent::Serialize(InSerializer);
        CClass* Class = CClassManager::GetInst().GetClass<CTransform>();
        assert(Class);
        CSerializer Data;
        Data["Position"] = { Position.x, Position.y, Position.z };
        Data["Rotation"] = { Rotation.x, Rotation.y, Rotation.z };
        Data["Scale"] = { Scale.x, Scale.y, Scale.z };
        Data["Speed"] = Speed;
        InSerializer = Data;
    }
    void Deserialize(const CSerializer& InDeserializer) override
    {
        CComponent::Deserialize(InDeserializer);
        Position.x = InDeserializer["Position"][0];
        Position.y = InDeserializer["Position"][1];
        Position.z = InDeserializer["Position"][2];
        Rotation.x = InDeserializer["Rotation"][0];
        Rotation.y = InDeserializer["Rotation"][1];
        Rotation.z = InDeserializer["Rotation"][2];
        Scale.x = InDeserializer["Scale"][0];
        Scale.y = InDeserializer["Scale"][1];
        Scale.z = InDeserializer["Scale"][2];
        Speed = InDeserializer["Speed"];
    }
    // =================================================================
    // Getters
    // =================================================================
    const Vector3& GetFinalPosition() const { return FinalPosition; }
    const Vector3& GetPosition() const { return Position; }
    const Vector3& GetRotation() const { return Rotation; }
    const Vector3& GetScale() const { return Scale; }
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
    void SetSpeed(float InSpeed) { Speed = InSpeed; }

    void Move(const Vector3& InDirection)
    {
        if (Speed == 0.0f)
            std::cout << "Speed Is Zero\n";
        SetPosition(Position + InDirection * Speed);
    }

    void SetVariation(bool bVariation) { bVariationFlag = bVariation; }
    bool OnVariation() const { return bVariationFlag; }

    void UpdateRenderState(class CRenderComponent& InRenderComponent);

private:
    Vector3 FinalPosition;
    Vector3 Position;
    Vector3 Rotation;
    Vector3 Scale;

    float Speed;

    bool bVariationFlag;
};