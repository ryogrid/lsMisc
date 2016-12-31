#pragma once
enum SHDELETEFLAG {
	SHDELETE_DEFAULT			= 0,
	SHDELETE_NOUI				= 1<<0,
	SHDELETE_COMPLETEDELETE		= 1<<1,
};
BOOL SHDeleteFile(LPCTSTR lpFile, SHDELETEFLAG shf=SHDELETE_DEFAULT);
