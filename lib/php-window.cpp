#include "php_window.h"
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "sciter-x-window.hpp"

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

