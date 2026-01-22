#include "pch.h"
#include "WzPartData.h"

#include "Utils.h"

bool ParseVectorObject(const rapidjson::GenericObject<true, rapidjson::Value>& InVectorObject, Vector2* OutWzVector)
{
	if (InVectorObject.HasMember("@name") == false)
		return false;
	if (InVectorObject.HasMember("@value") == false)
		return false;
	const std::string_view Name = InVectorObject["@name"].GetString();
	const std::string_view Value = InVectorObject["@value"].GetString();

	return StrToVec2(Value, OutWzVector);
}

bool ParsePartPngObject(const rapidjson::GenericObject<true, rapidjson::Value>& InPngObject, TWzPartData* OutPartData)
{
	if (InPngObject.HasMember("@value"))
		OutPartData->Value = InPngObject["@value"].GetString();

	if (InPngObject.HasMember("vector"))
	{
		const auto& VectorMember = InPngObject["vector"];
		if (VectorMember.IsArray())
		{
			const auto& VectorArray = VectorMember.GetArray();
			for (const auto& Vector : VectorArray)
			{
				if (Vector.IsObject() == false)
					return false;
				if (ParseVectorObject(Vector.GetObject(), &OutPartData->Origin) == false)
					return false;
			}
		}
		else if (VectorMember.IsObject())
			if (ParseVectorObject(VectorMember.GetObject(), &OutPartData->Origin) == false)
				return false;
	}


	if (InPngObject.HasMember("dir"))
	{
		// dir은 일단 map 하나만 있다고 가정
		const auto& DirMember = InPngObject["dir"];
		if (DirMember.IsObject())
		{
			const auto& DirObject = DirMember.GetObject();
			if (DirObject.HasMember("@name") == false)
				return false;
			const std::string_view Name = DirObject["@name"].GetString();
			if (Name != "map")
				return false;

			if (DirObject.HasMember("vector"))
			{
				const auto& VectorMember = DirObject["vector"];
				if (VectorMember.IsArray())
				{
					const auto& VectorArray = VectorMember.GetArray();
					for (const auto& Vector : VectorArray)
					{
						if (ParseMapData(Vector, OutPartData) == false)
							return false;
					}
				}
				else if (VectorMember.IsObject())
					return ParseMapData(VectorMember, OutPartData);
			}
		}
	}
	if (InPngObject.HasMember("string"))
	{
		const auto& StringMember = InPngObject["string"];
		if (StringMember.IsArray())
		{
			const auto& StringArray = StringMember.GetArray();
			for (const auto& String : StringArray)
			{
				if (String.IsObject() == false)
					return false;
				if (ParseStringData(String, OutPartData) == false)
					return false;
			}
		}
		else if (StringMember.IsObject())
			if (ParseStringData(StringMember, OutPartData) == false)
				return false;
	}
}

bool ParseStringData(const rapidjson::Value& InStringValue, TWzPartData* OutPartData)
{
	if (InStringValue.IsObject() == false)
		return false;

	const auto& StringObject = InStringValue.GetObject();
	if (StringObject.HasMember("@name") == false)
		return false;

	const std::string_view VectorName = StringObject["@name"].GetString();
	const std::string_view VectorValue = StringObject["@value"].GetString();

	if (VectorName == "z")
		OutPartData->Z = VectorValue;
	else if (VectorName == "group")
		OutPartData->Group = VectorValue;
	else if (VectorName == "_outlink")
		OutPartData->OutLink = "resources/image/" + std::string(VectorValue) + ".png";
}

bool ParseMapData(const rapidjson::Value& InVectorValue, TWzPartData* OutPartData)
{
	if (InVectorValue.IsObject() == false)
		return false;

	const auto& VectorObject = InVectorValue.GetObject();
	if (VectorObject.HasMember("@name") == false)
		return false;

	const std::string_view VectorName = VectorObject["@name"].GetString();
	Vector2* OutputData = nullptr;
	if (VectorName == "neck")
		OutputData = &OutPartData->Map.Neck;
	else if (VectorName == "navel")
		OutputData = &OutPartData->Map.Navel;
	else if (VectorName == "hand")
		OutputData = &OutPartData->Map.Hand;
	else if (VectorName == "handMove")
		OutputData = &OutPartData->Map.HandMove;

	if (ParseVectorObject(VectorObject, OutputData) == false)
		return false;

	return true;
}