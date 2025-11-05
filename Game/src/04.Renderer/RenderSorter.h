#pragma once
#include <algorithm>
#include "RenderStateObject.h"

class CRenderSorter
{
public:
	CRenderSorter() = default;
	~CRenderSorter() = default;

public:
	void SortByLayer(std::vector<CRenderStateObject*>& InRSOs)
	{
		std::sort(InRSOs.begin(), InRSOs.end(), [](CRenderStateObject* InA, CRenderStateObject* InB)->bool
			{
				return InA->GetRenderLayer() < InB->GetRenderLayer();
			});
	}

};

