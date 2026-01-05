#pragma once

#include <vector>

template <typename T>
bool EraseOnVector(std::vector<T*>& InVector, T& InErasedElement)
{
	for (auto Iter = InVector.begin(); Iter != InVector.end(); ++Iter)
	{
		if (*Iter == &InErasedElement)
		{
			InVector.erase(Iter);
			return true;
		}
	}
	return false;
}
