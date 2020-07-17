//
// Created by zhanglei on 2020/6/8.
//
//#include <ext/standard/basic_functions.h>
#include "sciter-common.h"

phpsciter::OpArrayCriticalSection::OpArrayCriticalSection()
{
    //create an consume thread
    if(!PHPSCITER_G(output_buffer).empty())
    {
        PHPSCITER_G(output_buffer).clear();
    }

    zend_hash_clean(&EG(symbol_table));
    //clear user function table
    clearUserGlobalFunctionTable();
    //clear user global class table
    clearUserGlobalClassTable();
    //clear non persistent constants
    cleanNonPersistentConstants();
    clearCallBack();
}

//clear user function table
void phpsciter::OpArrayCriticalSection::clearUserGlobalFunctionTable()
{
    //clear user function
#if PHP_VERSION_ID >= 70000
    zend_function *fptr;
    ZEND_HASH_REVERSE_FOREACH(CG(function_table), 0);
            fptr = (zend_function *)Z_PTR_P(_z);
            if(fptr->common.type == ZEND_INTERNAL_FUNCTION)
            {
                break;
            }else{
                zend_hash_del(EG(function_table) ,_p->key);
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

//clear non persistent constants
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
    zend_constant *constants;
    zend_hash_internal_pointer_end_ex(EG(zend_constants), &iterator);
    while (zend_hash_get_current_data_ex(EG(zend_constants), (void **) &constants, &iterator) == SUCCESS) {

        if(!iterator->arKey)
        {
            continue;
        }

        if(!constants->name)
        {
            continue;
        }

        if(constants->module_number != PHP_USER_CONSTANT)
        {
            break;
        }else{
            const char* constants_key = iterator->arKey;
            zend_hash_del(EG(zend_constants),  constants_key, strlen(constants_key) + 1);
        }
        zend_hash_move_backwards_ex(CG(function_table), &iterator);
    }
#endif
}

//clear user global class table
void phpsciter::OpArrayCriticalSection::clearUserGlobalClassTable()
{
    //clear user class table
#if PHP_VERSION_ID >= 70000

    zend_class_entry* ce;
    ZEND_HASH_REVERSE_FOREACH(CG(class_table), 0);
            ce = (zend_class_entry*)Z_PTR_P(_z);
            if(ce->type == ZEND_INTERNAL_CLASS)
            {
                break;
            }else{
                zend_hash_del(EG(class_table) ,_p->key);
            }
    ZEND_HASH_FOREACH_END();

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

#if PHP_VERSION_ID < 70000
static int user_shutdown_function_call(php_shutdown_function_entry *shutdown_function_entry TSRMLS_DC) /* {{{ */
{
    zval retval;
    char *function_name;

    if (!zend_is_callable(shutdown_function_entry->arguments[0], 0, &function_name TSRMLS_CC)) {
        php_error(E_WARNING, "(Registered shutdown functions) Unable to call %s() - function does not exist", function_name);
        if (function_name) {
            efree(function_name);
        }
        return 0;
    }
    if (function_name) {
        efree(function_name);
    }

    if (call_user_function(EG(function_table), NULL,
                           shutdown_function_entry->arguments[0],
                           &retval,
                           shutdown_function_entry->arg_count - 1,
                           shutdown_function_entry->arguments + 1
                           TSRMLS_CC ) == SUCCESS)
    {
        zval_dtor(&retval);
    }
    return 0;
}

static int zval_call_destructor(zval **zv TSRMLS_DC) /* {{{ */
{
    if (Z_TYPE_PP(zv) == IS_OBJECT && Z_REFCOUNT_PP(zv) == 1) {
        return ZEND_HASH_APPLY_REMOVE;
    } else {
        return ZEND_HASH_APPLY_KEEP;
    }
}

//php5 callable __destruct and shutdown function
static void call_destructors_and_shutdown_functions()
{
    /* 1. Call all possible shutdown functions registered with register_shutdown_function() */
    if (BG(user_shutdown_function_names)) {
        zend_try {
                    zend_hash_apply(BG(user_shutdown_function_names), (apply_func_t) user_shutdown_function_call TSRMLS_CC);
                }
        zend_end_try();
        if (BG(user_shutdown_function_names))
        zend_try {
            zend_hash_destroy(BG(user_shutdown_function_names));
            FREE_HASHTABLE(BG(user_shutdown_function_names));
            BG(user_shutdown_function_names) = NULL;
        } zend_catch {
            /* maybe shutdown method call exit, we just ignore it */
            FREE_HASHTABLE(BG(user_shutdown_function_names));
            BG(user_shutdown_function_names) = NULL;
        } zend_end_try();
    }

    //call destructors
    zend_try {
        int symbols;
        do {
            symbols = zend_hash_num_elements(&EG(symbol_table));
            zend_hash_reverse_apply(&EG(symbol_table), (apply_func_t) zval_call_destructor TSRMLS_CC);
        } while (symbols != zend_hash_num_elements(&EG(symbol_table)));
        zend_objects_store_call_destructors(&EG(objects_store) TSRMLS_CC);
    } zend_catch {
        /* if we couldn't destruct cleanly, mark all objects as destructed anyway */
        zend_objects_store_mark_destructed(&EG(objects_store) TSRMLS_CC);
    } zend_end_try();
}
#endif

phpsciter::OpArrayCriticalSection::~OpArrayCriticalSection()
{
    if(PHPSCITER_G(current_op_array))
    {
#if PHP_VERSION_ID >= 70000
        /* 1. Call all possible shutdown functions registered with register_shutdown_function() */
        if (PG(modules_activated)) zend_try {
            php_call_shutdown_functions();
        } zend_end_try();

        //call destructors
        zend_try {
            zend_call_destructors();
        } zend_end_try();

        php_free_shutdown_functions();

        //clear EG(object_store)
        zend_object **obj_ptr, **end, *obj;
        end = EG(objects_store).object_buckets + 1;
        obj_ptr = EG(objects_store).object_buckets + EG(objects_store).top;

        do {
            obj_ptr--;
            obj = *obj_ptr;
            if (IS_OBJ_VALID(obj)) {
                if (!(OBJ_FLAGS(obj) & IS_OBJ_FREE_CALLED)) {
                    GC_SET_REFCOUNT(obj, 0);
                    zend_objects_store_del(obj);
                }
            }
        } while (obj_ptr != end);
#else
        call_destructors_and_shutdown_functions();
#endif
        destroy_op_array(PHPSCITER_G(current_op_array));
        efree(PHPSCITER_G(current_op_array));
    }
    PHPSCITER_G(current_op_array) = nullptr;
}