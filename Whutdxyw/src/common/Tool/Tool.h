#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>
#include<tchar.h>
#include <vector>
#include <regex>
#include <sstream>
#include <codecvt>

#include <functional>

#include "../../../pch.h"

#define ERRORMSG	-1
#define INFOMSG		0

void ShowErrorMsg(int msgType, const char* pszFormat, ...);

wchar_t* ANSIToUnicode(const char* str);
char* UnicodeToANSI(const wchar_t* str);
wchar_t* UTF8ToUnicode(const char* str);
char* UnicodeToUTF8(const wchar_t* str);
char* ANSIToUTF8(const char* str);
char* UTF8ToANSI(const char* str);

std::vector<std::string> GetChildStr(std::string& sInput, std::string sStart, std::string sEnd);
std::string WCharToChar(CString cstr);
std::string urlEncode(const std::string& str);
std::wstring UTF8string(std::string strTemp);