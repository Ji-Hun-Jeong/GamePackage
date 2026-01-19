#pragma once
#include "Component.h"

class CTransform : public CComponent
{
	GENERATE_OBJECT(CTransform)
public:
	CTransform()
		: WorldPosition(Vector3(0.0f))
		, Position(Vector3(0.0f))
		, Rotation(Vector3(0.0f))
		, Scale(Vector3(1.0f))
	{
	}
	~CTransform() = default;

public:
	void Serialize(CSerializer& InSerializer) const override
	{
		CComponent::Serialize(InSerializer);
		/*CClass* Class = CClassManager::GetInst().GetClass<CTransform>();
		assert(Class);
		CSerializer Data;
		Data["Position"] = { Position.x, Position.y, Position.z };
		Data["Rotation"] = { Rotation.x, Rotation.y, Rotation.z };
		Data["Scale"] = { Scale.x, Scale.y, Scale.z };
		InSerializer = Data;*/
	}
	void Deserialize(const CSerializer& InDeserializer) override
	{
		CComponent::Deserialize(InDeserializer);
		/*Position.x = InDeserializer["Position"][0];
		Position.y = InDeserializer["Position"][1];
		Position.z = InDeserializer["Position"][2];
		Rotation.x = InDeserializer["Rotation"][0];
		Rotation.y = InDeserializer["Rotation"][1];
		Rotation.z = InDeserializer["Rotation"][2];
		Scale.x = InDeserializer["Scale"][0];
		Scale.y = InDeserializer["Scale"][1];
		Scale.z = InDeserializer["Scale"][2];*/
	}
	// =================================================================
	// Getters
	// =================================================================
	const Vector3& GetWorldPosition();
	const Vector3& GetPosition() const { return Position; }
	const Vector3& GetRotation() const { return Rotation; }
	const Vector3& GetScale() const { return Scale; }

	Vector2 GetWorldPosition2D();
	Vector2 GetPosition2D() const { return Vector2(Position.x, Position.y); }
	Vector2 GetScale2D() const { return Vector2(Scale.x, Scale.y); }

	// =================================================================
	// Setters
	// =================================================================
	void SetPosition(const Vector3& InPosition);
	void SetRotation(const Vector3& InRotation)
	{
		Rotation = InRotation;
	}
	void SetScale(const Vector3& InScale)
	{
		Scale = InScale;
	}

	void AddPositionOffset(const Vector3& InOffset)
	{
		PositionOffsets.emplace(InOffset);
		bChangePosition = true;
	}

	void Move(const Vector3& InOffset)
	{
		SetPosition(Position + InOffset);
	}

	static Vector3 GetDistVec3D(const Vector3& InStart, const Vector3& InEnd)
	{
		return InEnd - InStart;
	}
	static Vector2 GetDistVec2D(const Vector2& InStart, const Vector2& InEnd)
	{
		return InEnd - InStart;
	}

private:
	void ChangePosition();

private:
	Vector3 WorldPosition;
	Vector3 Position;
	Vector3 Rotation;
	Vector3 Scale;

	bool bChangePosition = true;

	std::queue<Vector3> PositionOffsets;

};

std::ostream& operator<< (std::ostream& InOS, const Vector3& InVec3);
std::ostream& operator<< (std::ostream& InOS, const Vector2& InVec2);