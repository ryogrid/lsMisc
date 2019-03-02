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

namespace Ambiesoft {


BYTE* UTF16toMultiByte(UINT cp, LPCWSTR pIN, size_t* pLenOut=NULL);
BYTE* UTF16toUTF8(LPCWSTR pIN);
LPWSTR UTF16_convertEndian(LPCWSTR pIN);
LPWSTR MultiBytetoUTF16(UINT cp, LPCSTR pIN);
bool MultiBytetoUTF16(UINT cp, LPCSTR pIN, std::wstring& wstr);
LPWSTR UTF8toUTF16(const LPBYTE pIN);
bool UTF8toUTF16(const LPBYTE pIN, std::wstring& w);

std::string toStdString(const std::wstring& w);
std::wstring toStdWstringFromUtf8(const std::string& s);


#ifdef __cplusplus_cli  
BYTE* UTF16toUTF8(System::String^ s);
#endif //__cplusplus_cli  


} // namespace