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

#include "php_window.h"
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "sciter-x-window.hpp"

void SciterWindowInit()
{

}

void SciterSetWindowTitle(HWINDOW hw, char* title)
{
    //gtk_window_set_title(GTK_WINDOW(hw), title);
}

void SciterShowWindow(HWINDOW hw)
{
    ShowWindow(hw, SW_NORMAL);
}

int SciterApplicationRun(HWINDOW hw)
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int) msg.wParam;
}

