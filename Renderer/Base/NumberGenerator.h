#pragma once
#include <queue>

namespace Graphics
{
	class CNumberGenerator
	{
	public:
		CNumberGenerator()
			: CurrentNumber(0)
		{}
		~CNumberGenerator() = default;

	public:
		size_t GenerateNumber()
		{
			if (ReuseValues.empty())
				return CurrentNumber++;

			size_t ReturnValue = ReuseValues.top();
			ReuseValues.pop();

			return ReturnValue;
		}

		void ReleaseNumber(size_t InNumber)
		{
			ReuseValues.push(InNumber);
		}

	private:
		std::priority_queue<size_t, std::vector<size_t>, std::greater<size_t>> ReuseValues;
		size_t CurrentNumber;
	};
}

