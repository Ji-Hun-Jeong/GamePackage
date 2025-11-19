#pragma once

#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>
#include <functional>
#include <Windows.h>

// 항상 Game쪽도 포함시켜줄것
#define DONTCOPY(Class) Class(const Class&) = delete; \
						Class(Class&&) = delete;\
						Class& operator = (const Class&) = delete;\
						Class& operator = (Class&&) = delete;

#define SINGLE(Class)	DONTCOPY(Class)\
						public:\
							static Class& GetInst() {static Class Instance;return Instance;}\
						private:\
							Class();