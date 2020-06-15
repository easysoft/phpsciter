//
// Created by zhanglei on 2020/6/8.
//
//#include <ext/standard/basic_functions.h>
#include "sciter-common.h"

phpsciter::OpArrayCriticalSection::OpArrayCriticalSection()
{
    zend_hash_clean(&EG(symbol_table));
    zend_rebuild_symbol_table();
    //clear user function table
    clearUserGlobalFunctionTable();
    //clear user global class table
    clearUserGlobalClassTable();
    //clear non persistent constants
    cleanNonPersistentConstants();
}

void phpsciter::OpArrayCriticalSection::clearUserGlobalFunctionTable()
{
    //clear user function
#if PHP_VERSION_ID >= 70000
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
    HashPosition iterator;
    zend_function *fptr;
    zend_hash_internal_pointer_end_ex(CG(function_table), &iterator);
    while (zend_hash_get_current_data_ex(CG(function_table), (void **) &fptr, &iterator) == SUCCESS) {
        if (fptr->common.type == ZEND_INTERNAL_FUNCTION) {
            break;
        }else if(fptr->common.type == ZEND_USER_FUNCTION){
            if(!iterator->arKey)
            {
                continue;
            }
            const char* function_name = iterator->arKey;
            zend_hash_del(CG(function_table), function_name, strlen(function_name) + 1);
        }
        zend_hash_move_backwards_ex(CG(function_table), &iterator);
    }
#endif
}

void phpsciter::OpArrayCriticalSection::cleanNonPersistentConstants()
{
#if PHP_VERSION_ID >= 70000
    zend_constant *val;
    ZEND_HASH_REVERSE_FOREACH(EG(zend_constants), 0);
        val = (zend_constant *)Z_PTR_P(_z);
        if(!val->name)
        {
            continue;
        }

        if (ZEND_CONSTANT_MODULE_NUMBER(val) == PHP_USER_CONSTANT) {
            zend_hash_del(EG(zend_constants) ,val->name);
        }else{
            break;
        }
    ZEND_HASH_FOREACH_END();
#else
    HashPosition iterator;
    zend_function *fptr;
    zend_hash_internal_pointer_end_ex(CG(function_table), &iterator);
    while (zend_hash_get_current_data_ex(CG(function_table), (void **) &fptr, &iterator) == SUCCESS) {
        if (fptr->common.type == ZEND_INTERNAL_FUNCTION) {
            break;
        }else if(fptr->common.type == ZEND_USER_FUNCTION){
            if(!iterator->arKey)
            {
                continue;
            }
            const char* function_name = iterator->arKey;
            zend_hash_del(CG(function_table), function_name, strlen(function_name) + 1);
        }
        zend_hash_move_backwards_ex(CG(function_table), &iterator);
    }
#endif
}

void phpsciter::OpArrayCriticalSection::clearUserGlobalClassTable()
{
    //clear user class table
#if PHP_VERSION_ID >= 70000
    int class_table_count = CG(class_table)->nNumUsed;
    Bucket* end = CG(class_table)->arData;
    Bucket* begin = CG(class_table)->arData + class_table_count;
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

#else

    HashPosition iterator;
    zend_class_entry **pce;
    zend_hash_internal_pointer_end_ex(CG(class_table), &iterator);
    while (zend_hash_get_current_data_ex(CG(class_table), (void **) &pce, &iterator) == SUCCESS) {
        if ((*pce)->type == ZEND_INTERNAL_CLASS) {
            break;
        }else if((*pce)->type == ZEND_USER_CLASS){
            if(!iterator->arKey)
            {
                continue;
            }
            const char* class_name = iterator->arKey;
            zend_hash_del(CG(class_table), class_name, strlen(class_name) + 1);
        }
        zend_hash_move_backwards_ex(CG(class_table), &iterator);
    }
#endif
}


phpsciter::OpArrayCriticalSection::~OpArrayCriticalSection()
{
    if(PHPSCITER_G(current_op_array))
    {
        /* 1. Call all possible shutdown functions registered with register_shutdown_function() */
//        if (PG(modules_activated)) zend_try {
//            php_call_shutdown_functions();
//        } zend_end_try();
        zend_try {
            zend_call_destructors();
        } zend_end_try();
        destroy_op_array(PHPSCITER_G(current_op_array));
        efree(PHPSCITER_G(current_op_array));
    }
    PHPSCITER_G(current_op_array) = nullptr;
}