//
// Created by zhanglei on 2020/6/8.
//

//#include <ext/standard/basic_functions.h>
#include <php_variables.h>
#include "sciter-common.h"

phpsciter::OpArrayCriticalSection::OpArrayCriticalSection()
{
    zend_hash_clean(&EG(symbol_table));
    zend_rebuild_symbol_table();
    clearUserGlobalFunctionTable();
    clearUserGlobalClassTable();
}

void phpsciter::OpArrayCriticalSection::clearUserGlobalFunctionTable()
{
    //clear user function
    int function_count = CG(function_table)->nNumUsed;
    Bucket* end = CG(function_table)->arData;
    Bucket* begin = CG(function_table)->arData + function_count;
    for(; begin != end; begin--)
    {
        if(begin->key)
        {
            zval *_z = &begin->val;
            if(_z->value.func->type == ZEND_INTERNAL_FUNCTION)
            {
                break;
            }else{
                zend_hash_del(CG(function_table) ,begin->key);
            }

        }else{
            continue;
        }
    }
}

void phpsciter::OpArrayCriticalSection::clearUserGlobalClassTable()
{
    //clear user function
    int function_count = CG(class_table)->nNumUsed;
    Bucket* end = CG(class_table)->arData;
    Bucket* begin = CG(class_table)->arData + function_count;
    for(; begin != end; begin--)
    {
        if(begin->key)
        {
            zval *_z = &begin->val;
            if(_z->value.ce->type == ZEND_INTERNAL_CLASS)
            {
                break;
            }else{
                zend_hash_del(CG(class_table) ,begin->key);
            }

        }else{
            continue;
        }
    }
}


phpsciter::OpArrayCriticalSection::~OpArrayCriticalSection()
{
    if(PHPSCITER_G(cureent_op_array))
    {
        /* 1. Call all possible shutdown functions registered with register_shutdown_function() */
//        if (PG(modules_activated)) zend_try {
//                php_call_shutdown_functions();
//        } zend_end_try();
//        zend_try {
//            zend_call_destructors();
//        } zend_end_try();
//
//        zend_hash_clean(&EG(symbol_table));
//        if(CG(function_table))
//        {
//            zend_hash_clean(EG(function_table));
//        }
        destroy_op_array(PHPSCITER_G(cureent_op_array));
        efree(PHPSCITER_G(cureent_op_array));
    }
    PHPSCITER_G(cureent_op_array) = nullptr;
}