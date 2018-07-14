#include "stdafx.h"
#include <string>
#include <limits>
#include <gtest/gtest.h>

#include "../stdwin32/stdwin32.h"



using namespace std;
using namespace stdwin32;


// TODO: move stdwin32 to stdosd when it's become os-dependant
TEST(stdwin32, PathCombine)
{
	EXPECT_STREQ(stdCombinePath(L"aaa", L"bbb").c_str(), L"aaa\\bbb");
	EXPECT_STREQ(stdCombinePath(L"z:\\aaa", L"bbb").c_str(), L"z:\\aaa\\bbb");
}