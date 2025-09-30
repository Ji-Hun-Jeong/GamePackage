#pragma once
#include "Object.h"

template <typename T>
class CObjectPtr;

template <typename T>
class CReleaseRef : public IObjectDestroy
{
public:
	CReleaseRef(CObjectPtr<T>* InObjectPtr)
		: ObjectPtr(InObjectPtr)
	{}

public:
	void SetObjectPtr(CObjectPtr<T>* InNewObjectPtr)
	{
		ObjectPtr = InNewObjectPtr;
	}

private:
	void OnDestroy(CObject& InObject) override
	{
		if (ObjectPtr)
			ObjectPtr->ReleaseObject();
	}

	CObjectPtr<T>* ObjectPtr;
};

template <typename T>
class CObjectPtr
{
	friend class CReleaseRef<T>;
public:
	CObjectPtr(T* InRefObject)
		: RefObject(InRefObject)
		, ReleaseRefEvent(nullptr)
	{
		ReleaseRefEvent = static_cast<CReleaseRef*>(RefObject->RegistObjectDestroyEvent(std::make_unique<CReleaseRef>(this)));
	}
	CObjectPtr(const T& InOther)
		: RefObject(InOther.RefObject)
		, ReleaseRefEvent(nullptr)
	{
		ReleaseRefEvent = static_cast<CReleaseRef*>(RefObject->RegistObjectDestroyEvent(std::make_unique<CReleaseRef>(this)));
	}
	CObjectPtr(T&& InOther)
		: RefObject(InOther.RefObject)
		, ReleaseRefEvent(InOther.ReleaseRefEvent)
	{
		ReleaseRefEvent->SetObjectPtr(this);
		InOther.ReleaseRefEvent = nullptr;
		InOther.RefObject = nullptr;
	}
	~CObjectPtr()
	{
		// 참조하는 오브젝트보다 먼저 사라질 때
		if (IsValid())
			RefObject->RemoveObjectDestroyEvent(ReleaseRefEvent);
	}

public:
	bool IsValid() const { return RefObject; }

	T* operator->() const
	{
		return static_cast<T*>(RefObject);
	}

	T& operator*() const
	{
		return *static_cast<T*>(RefObject);
	}

	T* Get() const
	{
		return static_cast<T*>(RefObject);
	}

private:
	void ReleaseObject() { RefObject = nullptr; }

private:
	T* RefObject;
	CReleaseRef<T>* ReleaseRefEvent;

};

