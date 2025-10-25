#pragma once

class IObjectReturn
{
	friend class CWorld;
	virtual class CObject* ReturnObject() = 0;
};

class CObjectCreator
{
public:
	CObjectCreator() = default;
	~CObjectCreator() = default;

public:
	void RequestCreate(IObjectReturn* InRequestCreate)
	{
		assert(InRequestCreate);
		RequestQueue.push(InRequestCreate);
	}

private:
	std::queue<IObjectReturn*> RequestQueue;

};

