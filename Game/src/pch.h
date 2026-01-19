#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cassert>
#include <Windows.h>
#include <unordered_map>
#include <list>
#include <queue>
#include <set>
#include <map>
#include <functional>
#include <algorithm>

#include "Common/Define.h"
#include "Common/Json.h"
#include "Renderer/pch.h"

/*
생포인터: 내가 이녀석의 생명주기를 관리할거지만 너가 이걸 삭제하지 않는다면 내가 너한테 참조를 줄 순 있어.

shared: 누구에게나 이녀석의 생명주기를 관리할 권한이 있고 참조도 가질 수 있어.

unique: 나만이 생명주기를 관리하고 참조를 넘겨주지 않는 이상 나만이 참조
*/