#include <windows.h>
#include <tchar.h>

#include <string>

#include "../tstring.h"
#include "../ChangeFilenamable2.h"


int main()
{
	TCHAR szT[] = "開発機構特別資料";
	tstring t = ChangeFilenamable2(szT);
	return 0;
}
