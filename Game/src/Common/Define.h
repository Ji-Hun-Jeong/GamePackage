#pragma once

#define DONTCOPY(Class) Class(const Class&) = delete; \
						Class(Class&&) = delete;\
						Class& operator = (const Class&) = delete;\
						Class& operator = (Class&&) = delete;

#define DONTASSN(Class) Class& operator = (const Class&) = delete;\
						Class& operator = (Class&&) = delete;

#define SINGLE(Class)	DONTCOPY(Class)\
						public:\
							static Class& GetInst() {static Class Instance;return Instance;}\
						private:\
							Class();