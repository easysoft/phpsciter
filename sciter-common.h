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

//c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

//c++
#include <iostream>
#include <vector>
#include <memory>
#include <memory>
#include <map>
#include <string>

#ifdef WINDOWS
#define uint UINT

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif
#endif

//zend
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
#ifndef PHP_VERSION_ID
#include "php_version.h"
#endif
#include <php.h>

//sciter-sdk
#include <zend_types.h>
#include <zend_compile.h>
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
#define PHPSCITER_FILE_HLEN 7
#define FINISH_EOF "\r\n\r\n\r\n\r\n"
#define FINISH_EOF_LEN sizeof("\r\n\r\n\r\n\r\n")

#ifdef PHP_WIN32
#define PHP_PHPSCITER_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define PHP_PHPSCITER_API __attribute__((visibility("default")))
#else
#define PHP_PHPSCITER_API
#endif


#include "sciter-nocopy.h"
#include "sciter-util.h"
#include "sciter-pipe.h"
#include "sciter-thread.h"
#include "php_sciter.h"
#include "phpsciter_application.h"
#include "sciter-zend-request.h"
#include "sciter-zend-api.h"
#include "sciter-op_rarray-life.h"
#include "phpsciter_global.h"



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
