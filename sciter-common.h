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

#ifndef SCITER_COMMON
#define SCITER_COMMON

#include <sciter-x-api.h>

#ifdef __unix__
//c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <syscall.h>

#endif

#ifdef WINDOWS
#define uint UINT

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

//visual studio x86 error
#ifdef PHP_WIN32
#define _STATIC_ASSERT(expr) typedef char __static_assert_t[ (expr)?(expr):1 ]
#endif
#endif

//zend
extern "C"
{
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <locale.h>
#include <php.h>
#include <php_ini.h>
#include <Zend/zend_API.h>
#include <ext/standard/info.h>
#include <zend_exceptions.h>
#include <php7_wrapper.h>
#include <zend_types.h>
#include <zend_compile.h>
#include <php_variables.h>

#ifdef ZTS
# include "TSRM.h"
#endif
#include <ext/standard/basic_functions.h>
#include <php_main.h>
}
#ifndef PHP_VERSION_ID
#include "php_version.h"
#endif

//c++
#include <iostream>
#include <vector>
#include <memory>
#include <memory>
#include <map>
#include <string>

#include <sciter-x-api.h>
#include <include/sciter-x.h>

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
#define PHPSCITER_PROPERTY_OPTION               "option"
#define PHPSCITER_PROPERTY_OPTION_VALUE         "option_value"

#define PHPSCITER_SW_CHILD                      "SW_CHILD"
#define PHPSCITER_SW_TITLEBAR                   "SW_TITLEBAR"
#define PHPSCITER_SW_RESIZEABLE                 "SW_RESIZEABLE"
#define PHPSCITER_SW_TOOL                       "SW_TOOL"
#define PHPSCITER_SW_CONTROLS                   "SW_CONTROLS"
#define PHPSCITER_SW_GLASSY                     "SW_GLASSY"
#define PHPSCITER_SW_ALPHA                      "SW_ALPHA"
#define PHPSCITER_SW_MAIN                       "SW_MAIN"
#define PHPSCITER_SW_POPUP                      "SW_POPUP"
#define PHPSCITER_SW_ENABLE_DEBUG               "SW_ENABLE_DEBUG"
#define PHPSCITER_SW_OWNS_VM                    "SW_OWNS_VM"

//option
#define PHPSCITER_SMOOTH_SCROLL                 "SCITER_SMOOTH_SCROLL"
#define PHPSCITER_CONNECTION_TIMEOUT            "SCITER_CONNECTION_TIMEOUT"
#define PHPSCITER_HTTPS_ERROR                   "SCITER_HTTPS_ERROR"
#define PHPSCITER_FONT_SMOOTHING                "SCITER_FONT_SMOOTHING"
#define PHPSCITER_TRANSPARENT_WINDOW            "SCITER_TRANSPARENT_WINDOW"
#define PHPSCITER_SET_GPU_BLACKLIST             "SCITER_SET_GPU_BLACKLIST"
#define PHPSCITER_SET_SCRIPT_RUNTIME_FEATURES   "SCITER_SET_SCRIPT_RUNTIME_FEATURES"
#define PHPSCITER_SET_GFX_LAYER                 "SCITER_SET_GFX_LAYER"
#define PHPSCITER_SET_DEBUG_MODE                "SCITER_SET_DEBUG_MODE"
#define PHPSCITER_SET_UX_THEMING                "SCITER_SET_UX_THEMING"
#define PHPSCITER_ALPHA_WINDOW                  "SCITER_ALPHA_WINDOW"
#define PHPSCITER_SET_INIT_SCRIPT               "SCITER_SET_INIT_SCRIPT"
#define PHPSCITER_SET_MAIN_WINDOW               "SCITER_SET_MAIN_WINDOW"
#define PHPSCITER_SET_MAX_HTTP_DATA_LENGTH      "SCITER_SET_MAX_HTTP_DATA_LENGTH"

//option value
#define PHPSCITER_ALLOW_FILE_IO                 "ALLOW_FILE_IO"
#define PHPSCITER_ALLOW_SOCKET_IO               "ALLOW_SOCKET_IO"
#define PHPSCITER_ALLOW_EVAL                    "ALLOW_EVAL"
#define PHPSCITER_ALLOW_SYSINFO                 "ALLOW_SYSINFO"

#define PHPSCITER_FILE_HLEN 7

#ifdef PHP_WIN32
#define PHP_PHPSCITER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define PHP_PHPSCITER_API __attribute__((visibility("default")))
#else
#define PHP_PHPSCITER_API
#endif


#include "sciter-nocopy.h"
#include "sciter-util.h"
#include "php_sciter.h"
#include "sciter-zend-request.h"
#include "sciter-zend-api.h"
#include "phpsciter_global.h"
#include "sciter-zend-hook.h"
#include "sciter-op_array-life.h"
#include "sciter-zend-hook.h"
#include "sciter-customer-log.h"
#include "phpsciter_application.h"


enum {
    CUSTOMER_ERROR = 1001,
};


PHP_MINIT_FUNCTION(phpsciter);
PHP_MSHUTDOWN_FUNCTION(phpsciter);
PHP_RINIT_FUNCTION(phpsciter);
PHP_RSHUTDOWN_FUNCTION(phpsciter);
PHP_MINFO_FUNCTION(phpsciter);
PHP_GINIT_FUNCTION(phpsciter);
PHP_GSHUTDOWN_FUNCTION(phpsciter);

#endif /* PHP_PHPsciter_H */
