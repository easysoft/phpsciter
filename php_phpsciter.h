/*
  +----------------------------------------------------------------------+
  | SeasLog                                                              |
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


#ifdef PHP_WIN32
#define PHP_PHPSCITER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define PHP_PHPSCITER_API __attribute__((visibility("default")))
#else
#define PHP_PHPSCITER_API
#endif

ZEND_BEGIN_MODULE_GLOBALS(phpsciter)
char *resource_base_path;
char *default_title;
zend_bool loadFile;
zend_bool loadHtml;
ZEND_END_MODULE_GLOBALS(phpsciter)

extern ZEND_DECLARE_MODULE_GLOBALS(phpsciter);

#ifdef ZTS
# define PHPSCITER_G(v) TSRMG(phpsciter_globals_id, zend_phpsciter_globals *, v)
#else
# define PHPSCITER_G(v) (phpsciter_globals.v)
#endif

PHP_MINIT_FUNCTION(phpsciter);
PHP_MSHUTDOWN_FUNCTION(phpsciter);
PHP_RINIT_FUNCTION(phpsciter);
PHP_RSHUTDOWN_FUNCTION(phpsciter);
PHP_MINFO_FUNCTION(phpsciter);

PHP_METHOD(phpsciter, __construct);
PHP_METHOD(phpsciter, __destruct);
PHP_METHOD(phpsciter, __sleep);
PHP_METHOD(phpsciter, __wakeup);
PHP_METHOD(phpsciter, __clone);
PHP_METHOD(phpsciter, getInstance);
PHP_METHOD(phpsciter, getVersion);
PHP_METHOD(phpsciter, getPHPSciterVersion);
PHP_METHOD(phpsciter, setResourcePath);
PHP_METHOD(phpsciter, setWindowFrame);
PHP_METHOD(phpsciter, setWindowTitle);
PHP_METHOD(phpsciter, loadFile);
PHP_METHOD(phpsciter, loadHtml);
PHP_METHOD(phpsciter, defineFunction);
PHP_METHOD(phpsciter, ifDefined);
PHP_METHOD(phpsciter, run);

PHP_GINIT_FUNCTION(phpsciter);
PHP_GSHUTDOWN_FUNCTION(phpsciter);


#endif /* PHP_PHPsciter_H */
