#pragma once

// HOW TO USE
// 1. Include I18N.h and I18N.cpp to your project.
// 2. Call i18nInitLangmap() at startup, or i18nInitLangmap(L"jpn") for specifying language.
// 3. Create a Folder named "lang" under the exe-residing-folder, and place a lang file in it 
//    and name it like "jpn.txt".
// 4. jpn.txt format is like follows:
//    "Hello"="こんにちは"
//    "Goodby"="さようなら"
// 5. Call I18N(L"Hello") in your code. It returns const "こんにちは".
// 6. Call i18Nxxxx() for translating windows texts. Show the source for more informations.
// 7. Look the debugger output when the program exists. Untranslated words will be printed.

namespace Ambiesoft {
LPCWSTR i18nInitLangmap(HINSTANCE hInst=NULL, LPCWSTR pLang = NULL);
LPCWSTR i18nGetCurrentLang();
LPCWSTR I18N(LPCWSTR pIN);

void i18nChangeWindowText(HWND hwnd);
void i18nChangeChildWindowText(HWND hwnd);
void i18nChangeMenuText(HMENU menu);


} // namespace Ambiesoft