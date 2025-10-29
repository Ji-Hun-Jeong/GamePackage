#pragma once
#include "Object.h"

class CClass
{
public:
	CClass(const std::string& InClassName, ObjectType InClassType, std::function<CObject* ()> InCreateFunc)
		: ClassName(InClassName)
		, ClassType(InClassType)
		, CreateFunc(InCreateFunc)
	{}
	~CClass() = default;

public:
	ObjectType GetClassType() const { return ClassType; }
	const std::string& GetName() const { return ClassName; }
	CObject* CreateObject() const { return CreateFunc(); }

private:
	std::string ClassName;
	ObjectType ClassType;
	std::function<CObject* ()> CreateFunc;

};

class CClassManager
{
	SINGLE(CClassManager)
public:
	template <typename T>
	CClass* GetClass() const
	{
		for (auto Iter = Classes.begin(); Iter != Classes.end(); ++Iter)
		{
			if (Iter->second->GetClassType() == T::GetStaticType())
				return Iter->second.get();
		}
		return nullptr;
	}
	CClass* GetClassByName(const std::string& InClassName) const
	{
		auto Iter = Classes.find(InClassName);
		if (Iter == Classes.end())
			return nullptr;
		return Iter->second.get();
	}
	void AddClass(std::unique_ptr<CClass> InClass)
	{
		Classes.insert({ InClass->GetName(), std::move(InClass) });
	}
private:
	std::map<std::string, std::unique_ptr<CClass>> Classes;

};

template <typename T>
class CClassRegister
{
public:
	CClassRegister(const std::string& InClassName)
	{
		ClassInstance = new CClass(InClassName, T::GetStaticType(), []()->CObject* { return new T; });
		CClassManager::GetInst().AddClass(std::move(std::unique_ptr<CClass>(ClassInstance)));
	}
public:
	CClass* GetClassInstance() const { return ClassInstance; }

private:
	CClass* ClassInstance;

};

#define GENERATE_OBJECT(Class) \
public:\
	static ObjectType GetStaticType()\
	{\
		static ObjectType Type = SetType();\
		return Type;\
	}\
	ObjectType GetType() const override {return GetStaticType();};\
private:\
	inline static CClassRegister<Class> ClassRegister{#Class};\
public:\
	static CClass* GetStaticClass() {return ClassRegister.GetClassInstance();}\
	CClass* GetClass() const override {return GetStaticClass();};\
