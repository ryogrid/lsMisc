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

#include <vector>
#include <string>
#include <memory>

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <wctype.h>

#include "stdosd_literal.h"

namespace Ambiesoft {
	namespace stdosd {


#define STDOSD_WCHARLITERAL_INNER(x) L ## x
#define STDOSD_WCHARLITERAL(x) STDOSD_WCHARLITERAL_INNER(x)

#define STDOSD_CHAR16TLITERAL_INNER(x) u ## x
#define STDOSD_CHAR16TLITERAL(x) STDOSD_CHAR16TLITERAL_INNER(x)

#if __GNUC__

    #define STDOSD_CONSTEXPR const constexpr
    #define CHAR16T_AVAILABLE

#elif _WIN32 // not __GNUC__ but _WIN32

    #if _MSC_VER <= 1800  // less than or equal to VC2013 ( or VC12 )
    #define STDOSD_CONSTEXPR const
    #else
    #define STDOSD_CONSTEXPR const constexpr
    #endif

    #if _MSC_VER >= 1900
    #define CHAR16T_AVAILABLE
    #endif

#endif // _WIN32 __GNUC__



#if defined(_MSC_VER) || defined(__MINGW32__)
#define STDOSD_DEFAULTSEPARATOR "\\"
#define STDOSD_PATHSEPARATORS "/\\"
#define STDOSD_NEWLINE "\r\n"
#else
#define STDOSD_DEFAULTSEPARATOR "/"
#define STDOSD_PATHSEPARATORS "/"
#define STDOSD_NEWLINE "\n"
#endif


        typedef void* HFILEITERATOR;
        class FileInfo
        {
            unsigned long long size_ = 0;
            std::string name_;
            std::string path_;
            bool isDir_ = false;
        public:
            unsigned long long size() const {
                return size_;
            }
            std::string name() const {
                return name_;
            }
//            std::string path() const {
//                return path_;
//            }
            bool isDirectory() const {
                return isDir_;
            }

            void setAll(bool isDir,
                        const std::string& name,
                        const unsigned long long& size)
            {
                isDir_ = isDir;
                name_ = name;
                size_=size;
            }
        };


		
		template<typename C>
		inline bool isEmptyString(const C* str, size_t len) {
			return (len == 0 || !str || str[0] == 0);
		}
		inline size_t getCharLength(const char* p) {
			return std::strlen(p);
		}
		inline size_t getCharLength(const wchar_t* p) {
			return wcslen(p);
		}

#if defined(_MSC_VER) || defined(__MINGW32__)
		inline static int myStrCaseCmp(const char* left, const char* right) {
			return _strcmpi(left, right);
		}
		inline static int myStrCaseCmpW(const wchar_t* left, const wchar_t* right) {
			return _wcsicmp(left, right);
		}
		inline static int myStrNCaseCmp(const char* left, const char* right, size_t len) {
			return _strnicmp(left, right, len);
		}
		inline static int myStrNCaseCmpW(const wchar_t* left, const wchar_t* right, size_t len) {
			return _wcsnicmp(left, right, len);
		}
#else
		inline static int myStrCaseCmp(const char* left, const char* right) {
			return strcasecmp(left, right);
		}
		inline static int myStrCaseCmpW(const wchar_t* left, const wchar_t* right) {
			return wcscasecmp(left, right);
		}
		inline static int myStrNCaseCmp(const char* left, const char* right, size_t len) {
			return strncasecmp(left, right, len);
		}
		inline static int myStrNCaseCmpW(const wchar_t* left, const wchar_t* right, size_t len) {
			return wcsncasecmp(left, right, len);
		}
#endif

		inline bool isCharEqual(const char* left, const char* right, bool ignoreCase=false) {
			// Linux GNU C dose not have _strcmpi
			return ignoreCase ? myStrCaseCmp(left,right)==0 : strcmp(left, right) == 0;
		}
		inline bool isCharEqual(const wchar_t* left, const wchar_t* right, bool ignoreCase = false) {
			return ignoreCase ? myStrCaseCmpW(left,right)==0 : wcscmp(left, right) == 0;
		}

		inline bool isCharEqualN(const char* left, const char* right, size_t len, bool ignoreCase = false) {
			return ignoreCase ? myStrNCaseCmp(left, right, len) == 0 : strncmp(left, right, len) == 0;
		}
		inline bool isCharEqualN(const wchar_t* left, const wchar_t* right, size_t len, bool ignoreCase = false) {
			return ignoreCase ? myStrNCaseCmpW(left, right, len) == 0 : wcsncmp(left, right, len) == 0;
		}



		inline bool isAsciiDigit(char c) {
			return ('0' <= c) && (c <= '9');
		}
		inline bool isAsciiDigit(wchar_t c) {
			return (L'0' <= c) && (c <= L'9');
		}


		template<typename C>
		inline bool stdIsAsciiDigit(const C* str, size_t len = -1)
		{
			if (isEmptyString(str, len))
				return false;

            if (len == static_cast<size_t>(-1))
				len = getCharLength(str);

			for (size_t i = 0; i < len; ++i)
			{
				if (!isAsciiDigit(str[i]))
					return false;
			}
			return true;
		}
		inline bool stdIsAsciiDigit(const std::wstring& s)
		{
			return stdIsAsciiDigit(s.c_str(), s.size());
		}
		inline bool stdIsAsciiDigit(const std::string& s)
		{
			return stdIsAsciiDigit(s.c_str(), s.size());
		}





		inline bool isTdigit(char c)
		{
			return !!isdigit(c);
		}
		inline bool isTdigit(wchar_t c)
		{
			return !!iswdigit(c);
		}
		template<typename C>
		inline bool stdIsTdigit(const C* str, size_t len = -1)
		{
			if (isEmptyString(str, len))
				return false;

            if (len == static_cast<size_t>(-1))
				len = getCharLength(str);

			for (size_t i = 0; i < len; ++i)
			{
				if (!isTdigit(str[i]))
					return false;
			}
			return true;
		}
		inline bool stdIsTdigit(const std::wstring& s)
		{
			return stdIsTdigit(s.c_str(), s.size());
		}
		inline bool stdIsTdigit(const std::string& s)
		{
			return stdIsTdigit(s.c_str(), s.size());
		}

	

		inline const wchar_t* getRChar(const wchar_t* p, wchar_t c)
		{
			return wcsrchr(p, c);
		}
		inline const char* getRChar(const char* p, char c)
		{
			return strrchr(p, c);
		}

		template<typename C>
		inline const C* getOneofRChar(const C* pOriginal, const C* pOneof)
		{
			const C* pRet = nullptr;
			// const C* pOriginal = pOneof;
			for (const C* pC = pOneof; *pC; ++pC)
			{
				const C* pT = getRChar(pOriginal, *pC);
				if (pT)
					++pT;
				if (pT > pRet)
					pRet = pT;
			}
			return pRet;
		}

		template<typename C>
		inline const C* stdGetFileName(const C* pPath)
		{
			if (!pPath)
				return nullptr;
			const C* pSeparator = getOneofRChar(pPath, stdLiterals<C>::pathSeparators());
			if (!pSeparator)
				return pPath;

			return pSeparator;
		}
		inline std::string stdGetFileName(const std::string& w)
		{
			return stdGetFileName(w.c_str());
		}
		inline std::wstring stdGetFileName(const std::wstring& w)
		{
			return stdGetFileName(w.c_str());
		}



		template<typename C>
		inline const C* stdGetFileExtension(const C* pPath)
		{
			if (!pPath)
				return nullptr;

			const C* pFilename = stdGetFileName(pPath);
			if (!pFilename)
				return nullptr;

			const C* pExt = getRChar(pFilename, stdLiterals<C>::period);
			if (!pExt)
				return stdLiterals<C>::nulString();

			return pExt;
		}
		inline std::string stdGetFileExtension(const std::string& w)
		{
			return stdGetFileExtension(w.c_str());
		}
		inline std::wstring stdGetFileExtension(const std::wstring& w)
		{
			return stdGetFileExtension(w.c_str());
		}


		template<typename C>
		inline std::basic_string<C, std::char_traits<C>, std::allocator<C>> 
			stdGetFileNameWitoutExtension(const C* pPath)
		{
			if (!pPath)
				return std::basic_string<C, std::char_traits<C>, std::allocator<C>>();

			const C* pFilename = stdGetFileName(pPath);
			if (!pFilename)
				return std::basic_string<C, std::char_traits<C>, std::allocator<C>>();

			const C* pExt = getRChar(pFilename, stdLiterals<C>::period);
			if (!pExt)
				return pFilename;

			std::basic_string<C, std::char_traits<C>, std::allocator<C>> ret;
            int len = (int)(pExt - pFilename);
            ret.assign(pFilename, static_cast<size_t>(len));
			return ret;
		}
		inline std::string stdGetFileNameWitoutExtension(const std::string& w)
		{
			return stdGetFileNameWitoutExtension(w.c_str());
		}
		inline std::wstring stdGetFileNameWitoutExtension(const std::wstring& w)
		{
			return stdGetFileNameWitoutExtension(w.c_str());
		}




		template<typename C>
		inline bool isEndwithSeparator(const std::basic_string<C, std::char_traits<C>, std::allocator<C>>& s)
		{
			if (s.empty())
				return false;

			C lastChar = s[s.length() - 1];
			for (const C* p = stdLiterals<C>::pathSeparators(); *p; ++p)
			{
				if (lastChar == *p)
					return true;
			}
			return false;
		}

		template<typename C>
		inline std::basic_string<C, std::char_traits<C>, std::allocator<C> >
			stdGetParentDirectory(const C* pPath, bool bAddSeparator = false)
		{
			using mys = std::basic_string<C, std::char_traits<C>, std::allocator<C>>;

			if (!pPath || pPath[0] == 0)
				return mys();

			

			mys s(pPath);
			size_t len = s.size();
			if (isEndwithSeparator(s))
			{
				s = s.substr(0, len - 1);
			}

			const C* pStart = s.c_str();
			const C* pSeparator = getOneofRChar(s.c_str(), stdLiterals<C>::pathSeparators());
			if (!pSeparator)
			{
				return mys();
			}

			if (!bAddSeparator)
				s.assign(pStart, pSeparator - 1);
			else
				s.assign(pStart, pSeparator);

			return s;
		}
		inline std::string stdGetParentDirectory(const std::string& w, bool bAddSeparator = false)
		{
			return stdGetParentDirectory(w.c_str(), bAddSeparator);
		}
		inline std::wstring stdGetParentDirectory(const std::wstring& w, bool bAddSeparator = false)
		{
			return stdGetParentDirectory(w.c_str(), bAddSeparator);
		}




		// TODO: test with Linux
		template<typename C>
		bool stdIsFullPath(const C* pD, bool allownetwork)
		{
			if (!pD || pD[0] == 0)
				return false;

			if (pD[0] == stdLiterals<C>::NSlash)
				return true;

			
			if (!((stdLiterals<C>::Na <= pD[0] && pD[0] <= stdLiterals<C>::Nz) ||
				(stdLiterals<C>::NA <= pD[0] && pD[0] <= stdLiterals<C>::NZ)))
			{
				if (!allownetwork)
					return false;

				if (pD[1] == 0)
					return false;
				
				if (!(pD[0] == stdLiterals<C>::NBackSlash && pD[1] == stdLiterals<C>::NBackSlash))
					return false;

				return pD[2] != 0;
			}

			if (pD[1] == stdLiterals<C>::NColon)
				return true;

			return false;
		}
		inline bool stdIsFullPath(const std::string& w, bool allownetwork = true)
		{
			return stdIsFullPath(w.c_str(), allownetwork);
		}
		inline bool stdIsFullPath(const std::wstring& w, bool allownetwork = true)
		{
			return stdIsFullPath(w.c_str(), allownetwork);
		}



		template<typename C>
		inline std::basic_string<C, std::char_traits<C>, std::allocator<C>>
		stdCombinePath(const C* pD1, const C* pD2)
		{
			using myS = std::basic_string<C, std::char_traits<C>, std::allocator<C>>;
			if (!pD1 || !pD1[0])
				return pD2;

			if (!pD2 || !pD2[0])
				return pD1;

			if (stdIsFullPath(pD2))
				return pD2;

			myS ret = pD1;
			if (!isEndwithSeparator(ret))
				ret += stdLiterals<C>::defaultSeparator(); // '\\'; // TODO to make this template

			ret += pD2;
			return ret;
		}
		inline std::wstring stdCombinePath(const std::wstring& d1, const std::wstring& d2)
		{
			return stdCombinePath(d1.c_str(), d2.c_str());
		}
		inline std::string stdCombinePath(const std::string& d1, const std::string& d2)
		{
			return stdCombinePath(d1.c_str(), d2.c_str());
		}



		template<typename StringType>
		inline StringType stdStringReplaceHelper(
			StringType str,
			const StringType& from,
			const StringType& to)
		{
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != StringType::npos)
			{
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
			}
			return str;
		}
		inline std::string stdStringReplace(
			std::string str,
			const std::string& from,
			const std::string& to)
		{
			return stdStringReplaceHelper(str, from, to);
		}
		inline std::wstring stdStringReplace(
			std::wstring str,
			const std::wstring& from,
			const std::wstring& to)
		{
			return stdStringReplaceHelper(str, from, to);
		}




		// https://stackoverflow.com/a/13172514
		template<typename StringType>
		inline std::vector<StringType> stdSplitStringHelper(
			const StringType& str,
			const StringType& delimiter)
		{
			std::vector<StringType> strings;

			size_t pos = 0;
			size_t prev = 0;
			while ((pos = str.find(delimiter, prev)) != StringType::npos)
			{
				strings.push_back(str.substr(prev, pos - prev));
				prev = pos + 1;
			}

			// To get the last substring (or only, if delimiter is not found)
			strings.push_back(str.substr(prev));

			return strings;
		}
		inline std::vector<std::string> stdSplitString(
			const std::string& str,
			const std::string& delimiter)
		{
			return stdSplitStringHelper(str, delimiter);
		}
        inline std::vector<std::wstring> stdSplitString(
            const std::wstring& str,
            const std::wstring& delimiter)
        {
            return stdSplitStringHelper(str, delimiter);
        }
        inline std::vector<std::u16string> stdSplitString(
            const std::u16string& str,
            const std::u16string& delimiter)
        {
            return stdSplitStringHelper(str, delimiter);
        }


		template<typename StringType>
		inline std::vector<StringType> stdSplitStringToLineHelper(const StringType& str)
		{
			using C = typename StringType::traits_type::char_type;

			StringType t = stdStringReplace(
				stdStringReplace(
					str,
					stdLiterals<C>::CRLF(),
					stdLiterals<C>::LF()),
				stdLiterals<C>::CR(),
				stdLiterals<C>::LF());

			return stdSplitString(t, stdLiterals<C>::LF());
		}
		inline std::vector<std::string> stdSplitStringToLine(const std::string& str)
		{
			return stdSplitStringToLineHelper(str);
		}
		inline std::vector<std::wstring> stdSplitStringToLine(const std::wstring& str)
		{
			return stdSplitStringToLineHelper(str);
		}


// varargs not supported under /clr
#ifndef __cplusplus_cli
		inline int stdSprintF(char *buffer,
							  size_t sizeOfBuffer,
							  size_t count,
							  const char *format,
							  va_list argptr)
		{
#if defined(__MINGW32__)

            // (void)count;
            return vsnprintf_s(buffer, sizeOfBuffer, count, format, argptr);
#elif defined(__GNUC__)
            return vsnprintf(buffer, sizeOfBuffer, format, argptr);

#elif _WIN32
#if _MSC_VER <= 1400
			int n = _vsnwprintf((wchar_t *)str.data(), size, fmt.c_str(), ap);
#else
			return _vsnprintf_s(buffer,sizeOfBuffer,count,format,argptr);
#endif // _WIN32
#endif // __GUNC__
		}
		inline int stdSprintF(wchar_t *buffer,
							  size_t sizeOfBuffer,
							  size_t count,
							  const wchar_t *format,
							  va_list argptr)
		{
#if defined(__MINGW32__)
            (void)count;
            return vswprintf_s(buffer, sizeOfBuffer, format, argptr);
#elif defined(__GNUC__)
            return vswprintf(buffer,sizeOfBuffer,format,argptr);

#elif _WIN32
#if _MSC_VER <= 1400
			int n = _vsnwprintf((wchar_t *)str.data(), size, fmt.c_str(), ap);
#else
			return _vsnwprintf_s(buffer,sizeOfBuffer,count,format,argptr);
#endif // _WIN32
#endif // __GUNC__
		}


		template<typename StringType>
		inline StringType stdFormatHelper(const StringType fmt, ...)
		{
			using C = typename StringType::traits_type::char_type;

            int size = (static_cast<int>(fmt.size())) * 2 + 50;   // Use a rubric appropriate for your code
			StringType str;
			va_list ap;
			while (1) {     // Maximum two passes on a POSIX system...
				str.resize(size);
				va_start(ap, fmt);

                int n = stdSprintF(const_cast<C*>(str.data()), size, size - 1, fmt.c_str(), ap);

				va_end(ap);
				if (n > -1 && n < size) {  // Everything worked
					str.resize(n);
					return str;
				}
				if (n > -1)  // Needed size returned
					size = n + 1;   // For null char
				else
					size *= 2;      // Guess at a larger size (OS specific)
			}
			return str;
		}
		template<typename... ARGS>
		inline std::string stdFormat(const std::string& fmt, ARGS... args)
		{
			return stdFormatHelper(fmt, args...);
		}
		template<typename... ARGS>
		inline std::wstring stdFormat(const std::wstring& fmt, ARGS... args)
		{
			return stdFormatHelper(fmt, args...);
		}
#endif // __cplusplus_cli








		template<typename C>
		inline bool hasEnding(const C* fullString, const C* ending, 
			size_t fullLen = -1, size_t endLen = -1,
			bool ignoreCase = false) {
			if (fullString == nullptr)
				return false;
			
			if (ending == nullptr || ending[0] == 0)
				return true;

            if (fullLen == static_cast<size_t>(-1))
				fullLen = getCharLength(fullString);
            if (endLen == static_cast<size_t>(-1))
				endLen = getCharLength(ending);

			if (fullLen < endLen)
				return false;
			
			const C* pFullStartPos = fullString + (fullLen - endLen);
			return isCharEqual(pFullStartPos, ending, ignoreCase);
		}
		template<typename C>
		inline bool hasEndingI(const C* fullString, const C* ending,
			size_t fullLen = -1, size_t endLen = -1) {
			return hasEnding(fullString, ending,
				fullLen, endLen, true);
		}
		inline bool hasEnding(const std::string& fullString, const std::string& ending) {
			return hasEnding(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}
		inline bool hasEndingI(const std::string& fullString, const std::string& ending) {
			return hasEndingI(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}
		inline bool hasEnding(const std::wstring& fullString, const std::wstring& ending) {
			return hasEnding(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}
		inline bool hasEndingI(const std::wstring& fullString, const std::wstring& ending) {
			return hasEndingI(
				fullString.c_str(),
				ending.c_str(),
				fullString.size(),
				ending.size()
			);
		}


		template<typename C>
		inline bool hasStarting(const C* fullString, const C* starting,
			size_t fullLen = -1, size_t startLen = -1,
			bool ignoreCase = false) {
			if (fullString == nullptr)
				return false;

			if (starting == nullptr || starting[0] == 0)
				return true;

            if (fullLen == static_cast<size_t>(-1))
				fullLen = getCharLength(fullString);
            if (startLen == static_cast<size_t>(-1))
				startLen = getCharLength(starting);

			if (fullLen < startLen)
				return false;

			return isCharEqualN(fullString, starting, startLen, ignoreCase);
		}
		template<typename C>
		inline bool hasStartingI(const C* fullString, const C* starting,
			size_t fullLen = -1, size_t startLen = -1) {
			return hasStarting(fullString, starting,
				fullLen, startLen, true);
		}
		inline bool hasStarting(const std::string& fullString, const std::string& starting) {
			return hasStarting(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}
		inline bool hasStartingI(const std::string& fullString, const std::string& starting) {
			return hasStartingI(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}
		inline bool hasStarting(const std::wstring& fullString, const std::wstring& starting) {
			return hasStarting(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}
		inline bool hasStartingI(const std::wstring& fullString, const std::wstring& starting) {
			return hasStartingI(
				fullString.c_str(),
				starting.c_str(),
				fullString.size(),
				starting.size()
			);
		}


		std::wstring stdGetFullPathName(const std::wstring& ws);

		std::wstring resolveLink(const std::wstring& fullstring);


		template<typename C>
        inline std::basic_string<C> stdAddDQIfNecessary(const C* fullString, size_t size = static_cast<size_t>(-1)) {
            if (size == static_cast<size_t>(-1))
				size = getCharLength(fullString);

			if (size == 0)
				return fullString;

			if (fullString[0] == stdLiterals<C>::NDoubleQuote &&
				fullString[size - 1] == stdLiterals<C>::NDoubleQuote)
				return fullString;

			bool hasSpace = false;
			for (size_t i = 0; i < size; ++i)
			{
				if (fullString[i] == stdLiterals<C>::NSpace)
				{
					hasSpace = true;
					break;
				}
			}

			if (!hasSpace)
				return fullString;

			return std::basic_string<C>() + stdLiterals<C>::NDoubleQuote + fullString + stdLiterals<C>::NDoubleQuote;
		}
		inline std::string stdAddDQIfNecessary(const std::string& fullString) {
			return stdAddDQIfNecessary(fullString.c_str(), fullString.size());
		}
		inline std::wstring stdAddDQIfNecessary(const std::wstring& fullString) {
			return stdAddDQIfNecessary(fullString.c_str(), fullString.size());
		}


		template<typename C>
		inline bool stdIsAsciiAlpha(const C c) {
			// Lowercase
			if (stdLiterals<C>::Na <= c && c <= stdLiterals<C>::Nz)
				return true;

			// Uppercase
			if (stdLiterals<C>::NA <= c &&c <= stdLiterals<C>::NZ)
				return true;

			return false;
		}

		template<typename C>
		inline bool stdIsAsciiSpace(const C c) {
			return c == stdLiterals<C>::NSpace;
		}


		inline void stdCopyString(char* pDst, size_t size, const char* pSrc)
		{
			strcpy_s(pDst, size, pSrc);
		}
		inline void stdCopyString(wchar_t* pDst, size_t size, const wchar_t* pSrc)
		{
			wcscpy_s(pDst, size, pSrc);
		}

		template<typename C>
		inline C* stdStringLower(C* pD1, size_t size);
		template<>
		inline char* stdStringLower(char* pc, size_t size)
		{
			_strlwr_s(pc, size + 1);
			return pc;
		}
		template<>
		inline wchar_t* stdStringLower(wchar_t* pwc, size_t size)
		{
			_wcslwr_s(pwc, size + 1);
			return pwc;
		}
		
		template<class C>
		inline std::basic_string<C> stdStringLower(const std::basic_string<C>& str)
		{
			std::unique_ptr<C[]> ptr(new C[str.size() + 1]);
			stdCopyString(ptr.get(), str.size() + 1, str.c_str());
			return stdStringLower(ptr.get(), str.size());
		}

        HFILEITERATOR stdCreateFileIterator(const std::string& directory);
        bool stdFileNext(HFILEITERATOR hFileIterator, FileInfo* fi);
        bool stdCloseFileIterator(HFILEITERATOR hFileIterator);




		template<typename C>
		inline std::basic_string<C> stdTrimStart(
			const std::basic_string<C>& str,
			const C* whitespace = stdLiterals<C>::WHITESPACE())
		{
            using ST = std::basic_string<C>;

			if (whitespace == nullptr || whitespace[0] == 0)
				return str;

            const typename ST::size_type strBegin = str.find_first_not_of(whitespace);
			if (strBegin == ST::npos)
				return ST(); // no content

			return str.substr(strBegin);
		}
		template<typename C>
		inline std::basic_string<C> stdTrimStart(
			const std::basic_string<C>& str,
			const std::basic_string<C>& whitespace)
		{
			return stdTrimStart(str, whitespace.c_str());
		}

		template<typename C>
		inline std::basic_string<C> stdTrimEnd(
			const std::basic_string<C>& str,
			const C* whitespace = stdLiterals<C>::WHITESPACE())
		{
			using ST = std::basic_string<C>;

			if (whitespace == nullptr || whitespace[0] == 0)
				return str;

			const typename ST::size_type strEnd = str.find_last_not_of(whitespace);
			return str.substr(0, strEnd + 1);
		}
		template<typename C>
		inline std::basic_string<C> stdTrimEnd(
			const std::basic_string<C>& str,
			const std::basic_string<C>& whitespace)
		{
			return stdTrimEnd(str, whitespace.c_str());
		}

		template<typename C>
		inline std::basic_string<C> stdTrim(
			const std::basic_string<C>& str,
			const C* whitespace = stdLiterals<C>::WHITESPACE())
		{
			using ST = std::basic_string<C>;

			if (whitespace == nullptr || whitespace[0] == 0)
				return str;

			const typename ST::size_type strBegin = str.find_first_not_of(whitespace);
			if (strBegin == ST::npos)
				return ST(); // no content

			const typename ST::size_type strEnd = str.find_last_not_of(whitespace);
			const typename ST::size_type strRange = strEnd - strBegin + 1;

			return str.substr(strBegin, strRange);
		}
		template<typename C>
		inline std::basic_string<C> stdTrim(
			const std::basic_string<C>& str,
			const std::basic_string<C>& whitespace)
		{
			return stdTrim(str, whitespace.c_str());
		}




		typedef void* HMODULEINSTANCE;
		size_t stdGetModuleFileNameImpl(HMODULEINSTANCE hInst, char* p, size_t size);
		size_t stdGetModuleFileNameImpl(HMODULEINSTANCE hInst, wchar_t* p, size_t size);

		template<typename C = wchar_t>
		std::basic_string<C> stdGetModuleFileName(HMODULEINSTANCE hInst = NULL)
		{
			C* p = nullptr;
			size_t size = 64;
			for (;;)
			{
				p = (C*)realloc(p, size * sizeof(C));
				if (stdGetModuleFileNameImpl(hInst, p, size) < size)
					break;

				// Make double the size of required memory
				size *= 2;
			}

			std::basic_string<C> ret = p;
			free((void*)p);
			return ret;
		}

		typedef void* HWINDOWHANDLE;
		template<typename C>
		bool stdGetClipboardText(HWINDOWHANDLE hWindow, std::basic_string<C>& result)
		{
			// TODO: implement
		}


		template<typename C>
		C* stdStrDup(const C* p)
		{
			if (!p)
				return nullptr;
			size_t count = getCharLength(p) + 1;
			C* ret = new C[count];
			stdCopyString(ret, count, p);
			return ret;
		}



		size_t stdExpandEnvironmentStringsImpl(const char* pIN, char* p, size_t size);
		size_t stdExpandEnvironmentStringsImpl(const wchar_t* pIN, wchar_t* p, size_t size);

		template<typename C = wchar_t>
		std::basic_string<C> stdExpandEnvironmentStrings(const C* pIN)
		{
			C* p = nullptr;
			size_t size = 64;
			for (;;)
			{
				p = (C*)realloc(p, size * sizeof(C));
				if (stdExpandEnvironmentStringsImpl(pIN, p, size) < size)
					break;

				// Make double the size of required memory
				size *= 2;
			}

			std::basic_string<C> ret = p;
			free((void*)p);
			return ret;
		}



		bool GetComputerNameT(char* p, size_t* pnLength);
		bool GetComputerNameT(wchar_t* p, size_t* pnLength);

		template<class C>
		bool stdGetComputerNameImpl(C* p, size_t size, size_t& outsize)
		{
			if (!GetComputerNameT(p, &size))
			{
				if (GetLastError() != ERROR_BUFFER_OVERFLOW)
					return false;
			}
			outsize = size;
			return true;
		}

		template<typename C = wchar_t>
		std::basic_string<C> stdGetComputerName()
		{
			C* p = nullptr;
			size_t size = 16;
			for (;;)
			{
				p = (C*)realloc(p, size * sizeof(C));
				size_t outsize;
				if (!stdGetComputerNameImpl(p, size, outsize))
					return std::basic_string<C>();
				if (size > outsize)
					break;

				// Make double the size of required memory
				size *= 2;
			}

			std::basic_string<C> ret = p;
			free((void*)p);
			return ret;
		}


		
	}
}
