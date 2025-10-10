#pragma once
#include <iostream>
#include "RHI.h"

namespace Graphics
{
	class CRHIManager
	{
	public:
		CRHIManager() = default;
		~CRHIManager() = default;

	public:
		void AddRHI(const std::string& InRHIName, std::unique_ptr<CRHI> InRHI)
		{
			if (RHIs.contains(InRHIName))
				std::cout << "CRenderResourceManager: Resource Is Already Contained\n";
			RHIs.insert({ InRHIName, std::move(InRHI) });
		}
		void RemoveRHI(const std::string& InRHIName)
		{
			RHIs.erase(InRHIName);
		}

		template <typename T_RHI>
		T_RHI* GetRHI(const std::string& InRHIName)
		{
			auto Iter = RHIs.find(InRHIName);
			if (RHIs.end() == Iter)
				return nullptr;

			CRHI* RHI = Iter->second.get();
			if (RHI->GetRHIType() != T_RHI::StaticType())
				return nullptr;

			return static_cast<T_RHI*>(RHI);
		}

	private:
		std::map<std::string, std::unique_ptr<CRHI>> RHIs;

	};
}