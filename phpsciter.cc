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

#include "sciter-common.h"


ZEND_DECLARE_MODULE_GLOBALS(phpsciter);


#ifdef COMPILE_DL_PHPSCITER
BEGIN_EXTERN_C()
ZEND_GET_MODULE(phpsciter)
END_EXTERN_C()
#endif



PHP_INI_BEGIN()
STD_PHP_INI_ENTRY("phpsciter.resource_base_path", "", PHP_INI_ALL, OnUpdateString, resource_base_path, zend_phpsciter_globals, phpsciter_globals)
STD_PHP_INI_ENTRY("phpsciter.default_title", "phpsciter", PHP_INI_ALL, OnUpdateString, default_title, zend_phpsciter_globals, phpsciter_globals)
PHP_INI_END()

PHP_GINIT_FUNCTION(phpsciter)
{
    shared_ptr<phpsciter::Util> util = make_shared<phpsciter::Util>();
    phpsciter_globals->tool =  util;
    shared_ptr<phpsciter::ZendSciterRequest> request = make_shared<phpsciter::ZendSciterRequest>();
    phpsciter_globals->request = request;
    shared_ptr<phpsciter::ZendApi> zend = make_shared<phpsciter::ZendApi>();
    phpsciter_globals->zend = zend;
}

PHP_GSHUTDOWN_FUNCTION(phpsciter)
{
    phpsciter_globals->tool.reset();
    phpsciter_globals->request.reset();
    phpsciter_globals->zend.reset();
    memset(phpsciter_globals, 0, sizeof(zend_phpsciter_globals));
}

PHP_MINIT_FUNCTION(phpsciter)
{

    REGISTER_INI_ENTRIES();

    load_phpsciter_application();
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(phpsciter)
{
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

PHP_RINIT_FUNCTION(phpsciter)
{
    initFunctions();
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(phpsciter)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(phpsciter)
{
    char *sciter_version;
    spprintf(&sciter_version, 0, "%d", SciterVersion(FALSE));
    php_info_print_table_start();
    php_info_print_table_header(2, "PHPSciter support", "enabled");
    php_info_print_table_row(2, "libsciter version", sciter_version);
    php_info_print_table_end();

    efree(sciter_version);

    DISPLAY_INI_ENTRIES();
}

const zend_function_entry phpsciter_functions[] =
{
    PHP_FE_END
};

#if ZEND_MODULE_API_NO >= 20050922
zend_module_dep phpsciter_deps[] =
{
    {NULL, NULL, NULL}
};
#endif

zend_module_entry phpsciter_module_entry =
{
#if ZEND_MODULE_API_NO >= 20050922
    STANDARD_MODULE_HEADER_EX, NULL,
    phpsciter_deps,
#else
    STANDARD_MODULE_HEADER,
#endif
    "PHPSciter",
    phpsciter_functions,
    PHP_MINIT(phpsciter),
    PHP_MSHUTDOWN(phpsciter),
    PHP_RINIT(phpsciter),
    PHP_RSHUTDOWN(phpsciter),
    PHP_MINFO(phpsciter),
    PHP_PHPSCITER_VERSION,
    PHP_MODULE_GLOBALS(phpsciter),
    PHP_GINIT(phpsciter),
    PHP_GSHUTDOWN(phpsciter),
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};
