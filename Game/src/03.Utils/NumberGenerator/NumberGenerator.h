#pragma once

class CNumberGenerator
{
	DONTCOPY(CNumberGenerator)
public:
	CNumberGenerator()
		: CurrentNumber(0)
	{}
	~CNumberGenerator();

public:
	UINT GenerateNumber()
	{
		if (ReuseValues.empty())
			return CurrentNumber++;

		UINT ReturnValue = ReuseValues.top();
		ReuseValues.pop();

		return ReturnValue;
	}

	void ReleaseNumber(UINT InNumber)
	{
		ReuseValues.push(InNumber);
	}

private:
	std::priority_queue<UINT, std::vector<UINT>, std::greater<UINT>> ReuseValues;
	UINT CurrentNumber;
};

