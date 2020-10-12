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

#import <Cocoa/Cocoa.h>
#import <AppKit/NSApplication.h>
#include "php_window.h"

void SciterWindowInit()
{

}

void SciterSetWindowTitle(HWINDOW hw, char * title)
{
    [[(NSView*)hw window] setTitle:[NSString stringWithUTF8String:title]];
}

void SciterShowWindow(HWINDOW hw)
{
    [[(NSView*)hw window] makeKeyAndOrderFront:nil];
}


int SciterApplicationRun(HWINDOW hw)
{
    NSApplication * application = [NSApplication sharedApplication];

    [[(NSView*)hw window] makeKeyAndOrderFront: nil];
    [application run];
    return 0;
}

