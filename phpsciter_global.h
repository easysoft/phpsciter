//
// Created by root on 20-3-7.
//

#ifndef PHPSCITER_PHPSCITER_GLOBAL_H
#define PHPSCITER_PHPSCITER_GLOBAL_H

#include <memory>

#include "include/util.h"

ZEND_BEGIN_MODULE_GLOBALS(phpsciter)
    uint8_t loadModal;
    zend_bool loadFile;
    zend_bool loadHtml;
    char *resource_base_path;
    char *default_title;
    zend_op_array *cureent_op_array;

    Util* tool;
ZEND_END_MODULE_GLOBALS(phpsciter)

extern ZEND_DECLARE_MODULE_GLOBALS(phpsciter);

#ifdef ZTS
# define PHPSCITER_G(v) TSRMG(phpsciter_globals_id, zend_phpsciter_globals *, v)
#else
# define PHPSCITER_G(v) (phpsciter_globals.v)
#endif

#endif //PHPSCITER_PHPSCITER_GLOBAL_H
