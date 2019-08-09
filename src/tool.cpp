/*
  +----------------------------------------------------------------------+
  | PHPSciter                                                            |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Chitao Gao  <neeke@php.net>                                  |
  +----------------------------------------------------------------------+
*/
#include "php_sciter.h"
#include "php_phpsciter.h"

int strlen16(char16_t* strarg)
{
    int count = 0;
    if(!strarg)
        return -1;
    char16_t* str = (char16_t *)strarg;
    while(*str)
    {
        count++;
        str++;
    }
    return count;
}

std::string U16toString(const std::u16string &wstr)
{
    std::string str = "";
    char cstr[4] = "\0";
    mbstate_t mbs;
    for (const auto& it: wstr)
    {
        memset (&mbs, 0, sizeof (mbs));
        memmove(cstr, "\0\0\0", 3);
        // c16rtomb (cstr, it, &mbs);
        wcrtomb (cstr, it, &mbs);
        str.append(std::string(cstr));
    }
    return str;
}

CHAR* ForwardToChar(LPCWSTR quote)
{
    CHAR* dest = new CHAR[1024];
    std::string str = "";
//#if defined(_WIN32)
//
//#else
//    std::u16string a(quote);
    str = U16toString((char16_t *)quote);
    dest = (CHAR *)str.data();
//#endif

    return dest;
}

LPCWSTR ForwardToWSTR(char* quote)
{
    aux::a2w as(quote);
    return LPCWSTR(as.c_str());
}

UINT ForwardToLength(char* quote)
{
    aux::a2w as(quote);
    return UINT(as.length());
}



