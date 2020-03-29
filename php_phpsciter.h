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

#ifndef PHP_PHPSCITER_H
#define PHP_PHPSCITER_H

#include <sciter-x-api.h>

extern "C"
{
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <locale.h>
#include "php.h"
#include "php_ini.h"
#include "Zend/zend_API.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
#include "php7_wrapper.h"
#ifdef ZTS
# include "TSRM.h"
#endif
}

#include "php_window.h"
extern zend_module_entry phpsciter_module_entry;
#define phpext_phpsciter_ptr &phpsciter_module_entry

#define PHPSCITER_NAME "phpsciter"
#define PHP_PHPSCITER_VERSION "0.3.0"

#define PHPSCITER_INSTANCE_DEFAULT              ((char *)"default")
#define PHPSCITER_INSTANCE_DEFAULT_LEN          strlen(PHPSCITER_INSTANCE_DEFAULT)

#define PHPSCITER_PROPERTY_RESOURCE_PATH        "resource_path"
#define PHPSCITER_PROPERTY_FRAME_TOP            "frame_top"
#define PHPSCITER_PROPERTY_FRAME_LEFT           "frame_left"
#define PHPSCITER_PROPERTY_FRAME_RIGHT          "frame_right"
#define PHPSCITER_PROPERTY_FRAME_BOTTOM         "frame_bottom"
#define PHPSCITER_PROPERTY_TITLE                "title"
#define PHPSCITER_PROPERTY_LOAD_FILE            "load_file"
#define PHPSCITER_PROPERTY_LOAD_HTML            "load_html"
#define PHPSCITER_LOOD_HOOK_NAME                "getSciterHtmlByPhpFile"
#define PHPSCITER_FILE_HLEN 7

#define ZEND_GET "_GET"
#define ZEND_GET_LEN 4
#define ZEND_POST "_POST"
#define ZEND_POST_LEN 5
#define ZEND_REQUEST "_REQUEST"
#define ZEND_REQUEST_LEN 8
#define ZEND_SERVER "_SERVER"
#define ZEND_SERVER_LEN strlen("_SERVER")
#define ZEND_SERVER_REQUEST_URI "REQUEST_URI"
#define ZEND_SERVER_REQUEST_URI_LEN 11
#define ZEND_COOKIE "_COOKIE"
#define ZEND_COOKIE_LEN 7

#ifdef PHP_WIN32
#define PHP_PHPSCITER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define PHP_PHPSCITER_API __attribute__((visibility("default")))
#else
#define PHP_PHPSCITER_API
#endif

#include "phpsciter_global.h"

PHP_MINIT_FUNCTION(phpsciter);
PHP_MSHUTDOWN_FUNCTION(phpsciter);
PHP_RINIT_FUNCTION(phpsciter);
PHP_RSHUTDOWN_FUNCTION(phpsciter);
PHP_MINFO_FUNCTION(phpsciter);
PHP_GINIT_FUNCTION(phpsciter);
PHP_GSHUTDOWN_FUNCTION(phpsciter);

#endif /* PHP_PHPsciter_H */
