#pragma once

class IWorldEvent
{
	friend class CWorldEventHandler;
	virtual void Dispatch() = 0;
};

class CWorldEventHandler
{
public:
	CWorldEventHandler() = default;
	~CWorldEventHandler() = default;

public:
	void DispatchEvents()
	{
		while (WorldEvents.empty() == false)
		{
			WorldEvents.front()->Dispatch();
			WorldEvents.pop();
		}
	}
	void PushWorldEvent(std::unique_ptr<IWorldEvent> InGlobalEvent)
	{
		WorldEvents.push(std::move(InGlobalEvent));
	}

private:
	std::queue<std::unique_ptr<IWorldEvent>> WorldEvents;

};

