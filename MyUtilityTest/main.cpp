#include <windows.h>
#include <tchar.h>

#include <string>

#include "../tstring.h"
#include "../ChangeFilenamable2.h"
#include "../IsFileExists.h"

int main()
{
	TCHAR szT[] = "開発機構特別資料";
	tstring t = ChangeFilenamable2(szT);

	BOOL b = IsFileExists("c:\\T\\aaa.txt");
	b = IsFileExists("c:\\T\\bbb.txt");
	b = IsFileExists("c:\\T\\ccc");
	return 0;
}
