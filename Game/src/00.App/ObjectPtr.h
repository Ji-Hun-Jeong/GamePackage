#pragma once

#include "Object.h" // CObject definition (must include AddDestroyEvent, RemoveDestroyEvent, DestroyEventHandle, INVALID_EVENT_HANDLE)
#include <functional> // Although the callback is stored by CObject, we might use it internally if needed
#include <cassert>    // for assert

template <typename T>
class CObjectPtr
{
	static_assert(std::is_base_of_v<CObject, T>, "T must derive from CObject");
public:
	// 1. Constructors
	//------------------

	CObjectPtr() noexcept
		: RefPtr(nullptr)
	{}

	// nullptr constructor
	CObjectPtr(std::nullptr_t) noexcept
		: CObjectPtr()
	{}

	// Raw pointer constructor (Takes ownership conceptually, registers event)
	explicit CObjectPtr(T* InPtr)
		: RefPtr(InPtr)
	{
		RegisterDestroyEvent(); // Register event upon creation from raw pointer
	}

	// Copy constructor
	CObjectPtr(const CObjectPtr& Other)
	{
		RefPtr = Other.RefPtr;
		RegisterDestroyEvent();
	}

	// Move constructor (noexcept is important for container performance)
	CObjectPtr(CObjectPtr&& Other) noexcept
	{
		RefPtr = Other.RefPtr;
		RegisterDestroyEvent();
		Other.Reset();
	}

	// 2. Destructor
	//-------------
	~CObjectPtr()
	{
		UnregisterDestroyEvent(); // Clean up event listener
	}


	// Copy assignment
	CObjectPtr& operator=(const CObjectPtr& Other)
	{
		if (this != &Other)
		{
			UnregisterDestroyEvent();
			RefPtr = Other.RefPtr;
			RegisterDestroyEvent();
		}
		return *this;
	}

	// Move assignment (noexcept is important)
	CObjectPtr& operator=(CObjectPtr&& Other) noexcept
	{
		if (this != &Other)
		{
			UnregisterDestroyEvent();

			RefPtr = Other.RefPtr;
			DetectObjectDestroy = Other.DetectObjectDestroy;

			Other.Reset();
		}
		return *this;
	}

	// nullptr assignment
	CObjectPtr& operator=(std::nullptr_t) noexcept
	{
		Reset();
		return *this;
	}

	// Raw pointer assignment
	CObjectPtr& operator=(T* InPtr)
	{
		UnregisterDestroyEvent();
		RefPtr = InPtr;
		RegisterDestroyEvent();
		return *this;
	}

	void Reset()
	{
		UnregisterDestroyEvent();

		RefPtr = nullptr;
		DetectObjectDestroy = std::function<void()>();
	}

	// Gets the raw pointer
	T* Get() const noexcept
	{
		return RefPtr;
	}

	// Dereference operator (*)
	T& operator*() const noexcept // No null check, like standard smart pointers
	{
		assert(RefPtr != nullptr && "Attempted to dereference a null CObjectPtr");
		return *RefPtr;
	}

	// Member access operator (->)
	T* operator->() const noexcept // No null check
	{
		assert(RefPtr != nullptr && "Attempted to access member of a null CObjectPtr");
		return RefPtr;
	}

	// Boolean conversion (checks if pointer is valid)
	explicit operator bool() const noexcept
	{
		return RefPtr != nullptr;
	}

	// Explicit validity check
	bool IsValid() const noexcept
	{
		return RefPtr != nullptr;
	}

	// Comparison operators
	friend bool operator==(const CObjectPtr& Lhs, const CObjectPtr& Rhs) noexcept {
		return Lhs.RefPtr == Rhs.RefPtr;
	}
	friend bool operator!=(const CObjectPtr& Lhs, const CObjectPtr& Rhs) noexcept {
		return !(Lhs == Rhs);
	}
	friend bool operator==(const CObjectPtr& Lhs, std::nullptr_t) noexcept {
		return Lhs.RefPtr == nullptr;
	}
	friend bool operator!=(const CObjectPtr& Lhs, std::nullptr_t) noexcept {
		return Lhs.RefPtr != nullptr;
	}

private:
	void RegisterDestroyEvent()
	{
		if (RefPtr)
		{
			DetectObjectDestroy = [this]()->void
				{
					RefPtr = nullptr;
				};
			RefPtr->AddObjectDestroyEvent(&DetectObjectDestroy);
		}
	}

	void UnregisterDestroyEvent()
	{
		if (RefPtr)
			RefPtr->RemoveObjectDestroyEvent(&DetectObjectDestroy);
	}

private:
	T* RefPtr = nullptr;
	std::function<void()> DetectObjectDestroy;

};

template <typename T>
bool operator==(const CObjectPtr<T>& Lhs, const T* Rhs) noexcept
{
	return Lhs.Get() == Rhs;
}
template <typename T>
bool operator!=(const CObjectPtr<T>& Lhs, const T* Rhs) noexcept
{
	return !(Lhs == Rhs);
}
// ... add Rhs == Lhs versions if needed ...