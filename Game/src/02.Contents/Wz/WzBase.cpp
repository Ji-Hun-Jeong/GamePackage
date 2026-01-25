#include "pch.h"
#include "WzBase.h"

namespace Wz
{
	CWzNode* GenerateWzData(const JValue& InValue)
	{
		CWzNode* NewNode = new CWzNode();

		if (InValue.IsObject())
		{
			const auto& Object = InValue.GetObject();
			for (const auto& Member : Object)
			{
				const std::string_view Name = Member.name.GetString();

				CWzNode* ChildNode = GenerateWzData(Member.value);
				if (ChildNode)
					NewNode->AddMember(Name, *ChildNode);
			}
		}
		else if (InValue.IsArray())
		{
			// 배열인 경우 (예: [ {..}, {..} ]) 
			// 키를 "0", "1", "2" 문자열로 변환하여 자식으로 편입
			const auto& Array = InValue.GetArray();
			for (rapidjson::SizeType i = 0; i < Array.Size(); ++i)
			{
				CWzNode* ChildNode = GenerateWzData(Array[i]);
				if (ChildNode)
					NewNode->AddMember(std::to_string(i), *ChildNode);
			}
		}
		else
		{
			if (InValue.IsString())
				NewNode->SetValue(InValue.GetString());
		}

		return NewNode;
	}
}