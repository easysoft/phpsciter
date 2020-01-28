
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

#ifndef __CPP_FILE__
#define __CPP_FILE__

#include "sciter-x-types.h"

void SciterWindowInit();

void SciterSetWindowTitle(HWINDOW hw, char * title);
void SciterShowWindow(HWINDOW hw);

int SciterApplicationRun(HWINDOW hw);

#endif
