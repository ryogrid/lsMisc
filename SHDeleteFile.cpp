#include <windows.h>
#include <tchar.h>
#include <assert.h>

#include "SHDeleteFile.h"

BOOL SHDeleteFile(LPCTSTR lpFile)
{
	size_t len = _tcslen(lpFile);
	if(!lpFile || lpFile[0]==0 || len <= 3)
		return FALSE;

	// フルパスのみ許す
	do
	{
#ifndef UNICODE
		if (IsDBCSLeadByte((BYTE)lpFile[0]))
			return FALSE;
#endif

		if( lpFile[0]==_T('\\') && lpFile[1]==_T('\\') )
			break;

		if( lpFile[1] == _T(':') && lpFile[2]==_T('\\') )
			break;

		return FALSE;
	} while(false);

	LPTSTR p = (LPTSTR)malloc(len + (2*sizeof(TCHAR)));
	if(!p)
		return FALSE;
	_tcscpy(p, lpFile);
	p[len+1]=0;

	SHFILEOPSTRUCT sfo = {0};
	sfo.hwnd = NULL;
	sfo.wFunc = FO_DELETE;
	sfo.pFrom = p;
	sfo.pTo   = NULL;  // ignored
	sfo.fFlags = FOF_ALLOWUNDO;

	int ret = SHFileOperation(&sfo);
	free(p);
	return ret==0;
}
