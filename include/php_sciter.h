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

#ifndef PHP_SCITER_H
#define PHP_SCITER_H


int strlen16(char16_t* strarg);
LPCWSTR ForwardToWSTR(char* quote);
UINT ForwardToLength(char* quote);
CHAR* ForwardToChar(LPCWSTR quote);

//#define CHAR2WSTR(quote, len) charToWSTR(quote, len)

char *getSciterString(const VALUE *p);
int getSciterInt(const VALUE *p);
VALUE NewValue();
BOOL SC_CALLBACK KeyValueCallbackElement(LPVOID param, const VALUE* pkey, const VALUE* pval);
UINT SetPHPValue(const VALUE* val, zval *item);
UINT SetSciterValue(VALUE *result, zval *entry);

UINT SC_CALLBACK SciterViewCallback(LPSCITER_CALLBACK_NOTIFICATION pns, LPVOID callbackParam );
BOOL SC_CALLBACK ElementEventProcImplementeation(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms);
BOOL initFunctions();
BOOL SciterExecuteFunction(HELEMENT he, SCRIPTING_METHOD_PARAMS* p);
BOOL functionRegister(zend_string *event_name, zval *callback);
BOOL checkRegisted(zend_string *event_name);
BOOL clearCallBack();
static void (*_zend_execute_ex) (zend_execute_data *execute_data);
#endif