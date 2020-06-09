//
// Created by zhanglei on 2020/6/8.
//

//#include <ext/standard/basic_functions.h>
#include <php_variables.h>
#include "sciter-common.h"

phpsciter::OpArrayCriticalSection::OpArrayCriticalSection()
{
    zend_rebuild_symbol_table();
    //将旧的栈清理掉
    zend_hash_clean(&EG(symbol_table));
    EG(symbol_table) = *zend_array_dup(PHPSCITER_G(storage_symbol_table));//赋值新的栈

//    zend_hash_destroy(CG(function_table));
    //efree(CG(function_table));
//    CG(function_table) = zend_array_dup(PHPSCITER_G(storage_function_table));
//    EG(function_table) = zend_array_dup(PHPSCITER_G(storage_function_table));
}

phpsciter::OpArrayCriticalSection::~OpArrayCriticalSection()
{
    if(PHPSCITER_G(cureent_op_array))
    {
        /* 1. Call all possible shutdown functions registered with register_shutdown_function() */
//        if (PG(modules_activated)) zend_try {
//                php_call_shutdown_functions();
//        } zend_end_try();
        zend_try {
            zend_call_destructors();
        } zend_end_try();

        destroy_op_array(PHPSCITER_G(cureent_op_array));
        efree(PHPSCITER_G(cureent_op_array));
    }
    PHPSCITER_G(cureent_op_array) = nullptr;
}