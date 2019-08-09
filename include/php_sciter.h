#ifndef PHP_SCITER_H
#define PHP_SCITER_H

#include "php_phpsciter.h"

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

#endif