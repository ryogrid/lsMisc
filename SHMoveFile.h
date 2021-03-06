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

#pragma once

#include <Windows.h>
#include <shellapi.h>
#include <vector>
#include <string>

namespace Ambiesoft {
	enum {
		default_fopFlags = FOF_ALLOWUNDO,
	};
	
	int SHMoveFile(HWND hWnd, LPCTSTR lpFileTo, LPCTSTR lpFileFrom, FILEOP_FLAGS fopFlags = default_fopFlags);
	int SHMoveFile(LPCTSTR lpFileTo, LPCTSTR lpFileFrom, FILEOP_FLAGS fopFlags = default_fopFlags);

	int SHCopyFile(HWND hWnd, LPCTSTR lpFileTo, LPCTSTR lpFileFrom, FILEOP_FLAGS fopFlags = default_fopFlags);
	int SHCopyFile(LPCTSTR lpFileTo, LPCTSTR lpFileFrom, FILEOP_FLAGS fopFlags = default_fopFlags);

	int SHDeleteFile(HWND hWnd, LPCTSTR lpFile, FILEOP_FLAGS fopFlags = default_fopFlags);
	int SHDeleteFile(LPCTSTR lpFile, FILEOP_FLAGS fopFlags = default_fopFlags);




	int SHMoveFile(HWND hWnd, LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, FILEOP_FLAGS fopFlags = default_fopFlags);
	int SHMoveFile(LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, FILEOP_FLAGS fopFlags = default_fopFlags);

	int SHCopyFile(HWND hWnd, LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, FILEOP_FLAGS fopFlags = default_fopFlags);
	int SHCopyFile(LPCTSTR lpFileTo, const std::vector<std::wstring>& sourcefiles, FILEOP_FLAGS fopFlags = default_fopFlags);

	int SHDeleteFile(HWND hWnd, const std::vector<std::wstring>& files, FILEOP_FLAGS fopFlags = default_fopFlags);
	int SHDeleteFile(const std::vector<std::wstring>& files, FILEOP_FLAGS fopFlags = default_fopFlags);


	
	
	int SHMoveFile(HWND hWnd, const std::vector<std::wstring>& destfiles, const std::vector<std::wstring>& sourcefiles, FILEOP_FLAGS fopFlags = default_fopFlags);
	int SHMoveFile(const std::vector<std::wstring>& destfiles, const std::vector<std::wstring>& sourcefiles, FILEOP_FLAGS fopFlags = default_fopFlags);

	int SHCopyFile(HWND hWnd, const std::vector<std::wstring>& destfiles, const std::vector<std::wstring>& sourcefiles, FILEOP_FLAGS fopFlags = default_fopFlags);
	int SHCopyFile(const std::vector<std::wstring>& destfiles, const std::vector<std::wstring>& sourcefiles, FILEOP_FLAGS fopFlags = default_fopFlags);

	std::wstring GetSHFileOpErrorString(int nRet);
}