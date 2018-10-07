#include "stdafx.h"
#include <string>
#include <limits>
#include <gtest/gtest.h>

#include "../stdwin32/stdwin32.h"



using namespace std;
using namespace stdwin32;




TEST(stdwin32, GetModuleFileNameTest)
{
	char szT[MAX_PATH];
	GetModuleFileNameA(NULL, szT, _countof(szT));
	string s = stdGetModuleFileNameA();
	EXPECT_STREQ(szT, s.c_str());

	wchar_t szTW[MAX_PATH];
	GetModuleFileNameW(NULL, szTW, _countof(szTW));
	wstring ws = stdGetModuleFileNameW();
	EXPECT_STREQ(szTW, ws.c_str());
}

