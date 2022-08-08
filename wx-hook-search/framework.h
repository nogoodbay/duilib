#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include "../DuiLib/UILib.h"
#include <fstream>

using namespace DuiLib;

typedef std::basic_ofstream<char16_t> u16ofstream;

void enumContainer(CControlUI* pControlUI, u16ofstream& stream);
