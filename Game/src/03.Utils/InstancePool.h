#pragma once

template <typename T>
class CInstancePool
{
public:
	CInstancePool()
	{
		InsertInstance(10);
	}
	~CInstancePool()
	{
		while (InstancePool.empty() == false)
		{
			T* Instance = InstancePool.front();
			InstancePool.pop();
			delete Instance;
		}
	}
public:
	T* GetInstance()
	{
		if (InstancePool.size() == 0)
			InsertInstance(10);
		
		T* Instance = InstancePool.front();
		InstancePool.pop();
		return Instance;
	}
	void ReturnInstance(T* InInstance)
	{
		InstancePool.push(InInstance);
	}
private:
	void InsertInstance(size_t InInstanceCount)
	{
		for (size_t i = 0; i < InInstanceCount; ++i)
			InstancePool.push(new T);
	}

private:
	std::queue<T*> InstancePool;

};
