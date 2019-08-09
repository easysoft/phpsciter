#import <Cocoa/Cocoa.h>
#import <AppKit/NSApplication.h>
#include <stdlib.h>
#include "php_window.h"

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

