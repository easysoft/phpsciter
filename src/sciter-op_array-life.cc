//
// Created by zhanglei on 2020/6/8.
//

//#include <ext/standard/basic_functions.h>
#include <php_variables.h>
#include "sciter-common.h"

phpsciter::OpArrayCriticalSection::OpArrayCriticalSection()
{

    zend_rebuild_symbol_table();
}

void phpsciter::OpArrayCriticalSection::clearUserGlobalFunctionTable()
{
#if PHP_VERSION_ID >= 70000
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
#else
    Bucket* begin = CG(function_table)->pListTail;

#endif
}

void phpsciter::OpArrayCriticalSection::clearUserGlobalClassTable()
{
    //clear user function
//    int function_count = CG(class_table)->nNumUsed;
//    Bucket* end = CG(class_table)->arData;
//    Bucket* begin = CG(class_table)->arData + function_count;
//    for(; begin != end; begin--)
//    {
//        if(begin->key)
//        {
//            zval *_z = &begin->val;
//            if(_z->value.ce->type == ZEND_INTERNAL_CLASS)
//            {
//                break;
//            }else{
//                zend_hash_del(CG(class_table) ,begin->key);
//            }
//
//        }else{
//            continue;
//        }
//    }
}


phpsciter::OpArrayCriticalSection::~OpArrayCriticalSection()
{
    if(PHPSCITER_G(current_op_array))
    {
        /* 1. Call all possible shutdown functions registered with register_shutdown_function() */
//        if (PG(modules_activated)) zend_try {
//                php_call_shutdown_functions();
//        } zend_end_try();
        zend_try {
            zend_call_destructors();
        } zend_end_try();
        clearUserGlobalFunctionTable();
        clearUserGlobalClassTable();
        zend_hash_clean(&EG(symbol_table));
        destroy_op_array(PHPSCITER_G(current_op_array));
        efree(PHPSCITER_G(current_op_array));
    }
    PHPSCITER_G(current_op_array) = nullptr;
}