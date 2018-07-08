//Copyright (C) 2017 Ambiesoft All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//1. Redistributions of source code must retain the above copyright
//notice, this list of conditions and the following disclaimer.
//2. Redistributions in binary form must reproduce the above copyright
//notice, this list of conditions and the following disclaimer in the
//documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
//FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//SUCH DAMAGE.

#include "stdafx.h"

#include <windows.h>
#include <tchar.h>
#include <Shobjidl.h>
#include <Objbase.h>
#include <ShlGuid.h>
#include <comdef.h>
#include <shlwapi.h>

#include "SHMoveFile.h"

#include "CreateShortcutFile.h"

#pragma comment(lib, "shlwapi.lib")

namespace Ambiesoft {

static BOOL CheckShortcutFile(LPCTSTR pszShortcutFile, 
							   LPCTSTR pszTargetFile,
							   LPCTSTR pszCurDir,
							   LPCTSTR pszArg,
						       int iIconLocation)
{
	BOOL bFailed = TRUE;
	if (!pszArg)
		pszArg = L"";
	if (!pszCurDir)
		pszCurDir = L"";
	HRESULT hr;
	IShellLinkWPtr pShellLink = NULL;
	CoInitialize(NULL);
    hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pShellLink);
	int bufflen = std::max(lstrlen(pszShortcutFile), lstrlen(pszTargetFile));
	bufflen = std::max(bufflen, lstrlen(pszCurDir));
	bufflen = std::max(bufflen, lstrlen(pszArg));
	bufflen++;
	LPTSTR pBuffer = (LPTSTR)malloc(bufflen * sizeof(TCHAR));
	if(SUCCEEDED(hr) && pShellLink != NULL)
	{
		IPersistFilePtr pPFile = NULL;
		hr = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPFile);
		if(SUCCEEDED(hr) && pPFile != NULL)
		{
			hr=pPFile->Load(pszShortcutFile, 0);
			if(SUCCEEDED(hr))
			{
				bFailed = FALSE;
				WIN32_FIND_DATA wfd;
				if(FAILED(pShellLink->GetPath(pBuffer,bufflen,&wfd,0)))
					return FALSE;
				

				int iI=0;
				if(FAILED(pShellLink->GetIconLocation( pBuffer, bufflen, &iI )))
					return FALSE;
				if(lstrcmp(pBuffer, pszTargetFile))
					return FALSE;
				if( iI != iIconLocation )
					return FALSE;


				if( FAILED(pShellLink->GetArguments(pBuffer, bufflen)) )
					return FALSE;
				if(lstrcmp(pBuffer,pszArg)!=0)
					return FALSE;

				
				if(FAILED(pShellLink->GetWorkingDirectory(pBuffer,bufflen)))
					return FALSE;
				if( lstrcmp(pBuffer,pszCurDir) !=0)
					return FALSE;
			}
		}
	}
	free(pBuffer);
	CoUninitialize();
	return !bFailed;
}

BOOL CreateShortcutFile(HWND hWnd,
								LPCTSTR pszShortcutFile, 
							   LPCTSTR pszTargetFile,
							   LPCTSTR pszCurDir,
							   LPCTSTR pszArg,
						       int iIconLocation)
{
	BOOL bFailed = TRUE;
	HRESULT hr;

	TCHAR szTempDir[MAX_PATH] = {0};
	DWORD dwTP = GetTempPath(MAX_PATH, szTempDir);
	if(dwTP==0 || !PathIsDirectory(szTempDir))
	{
		return FALSE;
	}

	TCHAR szTempFile[MAX_PATH] = {0};
	CoInitialize(NULL);
	{
		IShellLinkWPtr pShellLink = NULL;
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&pShellLink);
		if(SUCCEEDED(hr) && pShellLink != NULL)
		{
			IPersistFilePtr pPFile = NULL;
			hr = pShellLink->QueryInterface(IID_IPersistFile, (LPVOID*)&pPFile);
			if(SUCCEEDED(hr) && pPFile != NULL)
			{
				hr = pShellLink->SetPath(pszTargetFile);
				if(SUCCEEDED(hr))
				{
					bFailed = FALSE;
					if(iIconLocation != -1)
					{
						bFailed |= FAILED(pShellLink->SetIconLocation(pszTargetFile, iIconLocation));
					}

					if(pszArg)
						bFailed |= FAILED(pShellLink->SetArguments(pszArg));

					if (pszCurDir)
						bFailed |= FAILED(pShellLink->SetWorkingDirectory(pszCurDir));

					
					if(0==GetTempFileName(szTempDir, L"cbs", 0, szTempFile))
						return FALSE;

					bFailed |= FAILED(pPFile->Save(szTempFile, TRUE));
				}
			}
		}
	}
	CoUninitialize();
	if( bFailed )
		return FALSE;

	if(!CheckShortcutFile(szTempFile, 
							  pszTargetFile,
							  pszCurDir,
							  pszArg,
						      iIconLocation))
	{
		DeleteFile(szTempFile);
		return FALSE;
	}

	//if(!MoveFileEx(szTempFile, pszShortcutFile, MOVEFILE_REPLACE_EXISTING))
	//	return FALSE;

	if (0 != SHMoveFile(hWnd, pszShortcutFile, szTempFile, FOF_NOCONFIRMATION))
		return FALSE;

	return TRUE;
}

} // namespace