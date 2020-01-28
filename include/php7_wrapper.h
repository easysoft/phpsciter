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

#if PHP_VERSION_ID >= 70000

#define Z_TYPE_PP(zval_p)			Z_TYPE(**(zval_p))

#define PHPSCITER_STR_SIZE(s) sizeof(s) - 1
#define PHPSCITER_ZVAL_STRING(z, s) ZVAL_STRING(z, s)
#define PHPSCITER_ZVAL_STRINGL(z, s, l) ZVAL_STRINGL(z, s, l)
#define PHPSCITER_RETURN_STRINGL(k, l) RETURN_STRINGL(k, l)
#define PHPSCITER_ADD_INDEX_STRINGL(z, i, s, l) add_index_stringl(&z, i, s, l)
#define PHPSCITER_ADD_INDEX_LONG(z, i, l) add_index_long(&z, i, l)
#define PHPSCITER_ADD_INDEX_ZVAL(z, i, zn) add_index_zval(z, i, zn)
#define PHPSCITER_ADD_ASSOC_LONG_EX(z, s, l, v) add_assoc_long_ex(&z, s, l, (long) v)
#define PHPSCITER_ADD_ASSOC_DOUBLE_EX(z, s, l, v) add_assoc_double_ex(&z, s, l, (double) v)
#define PHPSCITER_ADD_ASSOC_ZVAL_EX(z, s, l, zn) add_assoc_zval_ex(z, s, l, zn)
#define PHPSCITER_ADD_ASSOC_ZVAL_EX_AND(z, s, l, zn) add_assoc_zval_ex(&z, s, l, zn)
#define PHPSCITER_ADD_ASSOC_STRING_EX(a, k, l, s) add_assoc_string_ex(&a, k, l, s)
#define PHPSCITER_ADD_NEXT_INDEX_STRING(a, s) add_next_index_string(a, s)
#define PHPSCITER_ADD_NEXT_INDEX_STRINGL(a, s, l) add_next_index_stringl(a, s, l)
#define PHPSCITER_ZEND_HASH_GET_CURRENT_KEY(ht, key, idx) zend_hash_get_current_key(ht, key, idx)
#define PHPSCITER_ZEND_HASH_INDEX_UPDATE(ht, h, pData, nDataSize, pDest)  zend_hash_index_update_ptr(ht, h, pData)

#define PHPSCITER_ZEND_UPDATE_PROPERTY(ce,z,zl,zn) zend_update_property(ce, z, zl, zn)
#define PHPSCITER_ZEND_UPDATE_PROPERTY_NULL(ce,z,zl) zend_update_property_null(ce, z, zl)
#define PHPSCITER_ZEND_UPDATE_PROPERTY_EX(ce,z,zl,zn) zend_update_property_ex(ce, z, zl, zn)
#define PHPSCITER_ZEND_UPDATE_STATIC_PROPERTY(ce,zl,zn) zend_update_static_property(ce, zl, &zn)
#define PHPSCITER_ZEND_UPDATE_PROPERTY_LONG(ce,z,zl,zn) zend_update_property_long(ce,z,zl,zn TSRMLS_CC)
#define PHPSCITER_ZEND_UPDATE_PROPERTY_STRING(ce,z,zl,s) zend_update_property_string(ce,z,zl,s)
#define PHPSCITER_ZEND_READ_PROPERTY(ce,z,sl) zend_read_property(ce,z,sl,0,NULL)

static inline int PHPSCITER_ZEND_IS_CALLBACK(zval *cb, int a, char **name)
{
    zend_string *key = NULL;
    int ret = zend_is_callable(cb, a, &key);
    char *tmp = (char *)emalloc(key->len);
    memcpy(tmp, key->val, key->len);
    zend_string_release(key);
    *name = tmp;
    return ret;
}

static inline int PHPSCITER_ZEND_IS_CALLBACK_EX(zval *callable, zval *object, uint check_flags, char **callable_name, int *callable_name_len, zend_fcall_info_cache *fcc, char **error TSRMLS_DC)
{
    zend_string *key = NULL;
    char *tmp = (char *)emalloc(key->len);
    int ret = zend_is_callable_ex(callable, NULL, check_flags, &key, fcc, error);
    memcpy(tmp, key->val, key->len);
    zend_string_release(key);
    *callable_name = tmp;
    return ret;
}

static zend_string *TS_S_L(char *s)
{
    zend_string *result;
    result = zend_string_init(s, strlen(s), 1);
    return result;
}

#define PHPSCITER_ZEND_CALL_METHOD_WITH_0_PARAMS(obj, ptr, what, method, retval)               zend_call_method_with_0_params(*obj,ptr,what,method,*retval)
#define PHPSCITER_ZEND_CALL_METHOD_WITH_1_PARAMS(obj, ptr, what, method, retval, v1)           zend_call_method_with_1_params(*obj,ptr,what,method,*retval,v1)
#define PHPSCITER_ZEND_CALL_METHOD_WITH_2_PARAMS(obj, ptr, what, method, retval, name, cb)     zend_call_method_with_2_params(*obj,ptr,what,method,*retval,name,cb)

#define PHPSCITER_PHP_MAX_PARAMS_NUM 20
static inline int PHPSCITER_CALL_USER_FUNCTION_EX(HashTable *function_table, zval** object_pp, zval *function_name, zval **retval_ptr_ptr, uint32_t param_count, zval *params, int no_separation, HashTable* ymbol_table)
{
    zval real_params[PHPSCITER_PHP_MAX_PARAMS_NUM];
    int i = 0;
    for (; i < (int)param_count; i++)
    {
        real_params[i] = params[i];
    }
    zval phpng_retval;
    *retval_ptr_ptr = &phpng_retval;
    zval *object_p = (object_pp == NULL) ? NULL : *object_pp;
    return call_user_function_ex(function_table, object_p, function_name, &phpng_retval, param_count, params, no_separation, NULL);
}

#define PHPSCITER_MAKE_STD_ZVAL(p) zval _stack_zval_##p; p = &(_stack_zval_##p)
#define PHPSCITER_ALLOC_INIT_ZVAL(p) do{p = (zval *)emalloc(sizeof(zval)); bzero(p, sizeof(zval));}while(0)
#define PHPSCITER_COPY_TO_STACK(a, b) {zval *__tmp = a; a = &b; memcpy(a, __tmp, sizeof(zval));}
#define PHPSCITER_ZVAL_PTR_DTOR(p)  zval_ptr_dtor(*p)

#else

#define PHPSCITER_STR_SIZE(s) strlen(s) + 1
#define PHPSCITER_ZVAL_STRING(z, s) ZVAL_STRING(z, s, 1)
#define PHPSCITER_ZVAL_STRINGL(z, s, l) ZVAL_STRING(z, s, l, 1)
#define PHPSCITER_RETURN_STRINGL(k, l) RETURN_STRINGL(k, l, 1)
#define PHPSCITER_ADD_INDEX_STRINGL(z, i, s, l) add_index_stringl(z, i, s, l, 1)
#define PHPSCITER_ADD_INDEX_LONG(z, i, l) add_index_long(z, i, l)
#define PHPSCITER_ADD_INDEX_ZVAL(z, i, zn) add_index_zval(z, i, zn)
#define PHPSCITER_ADD_ASSOC_LONG_EX(z, s, l, v) add_assoc_long_ex(z, s, l, (long) v)
#define PHPSCITER_ADD_ASSOC_DOUBLE_EX(z, s, l, v) add_assoc_double_ex(z, s, l, (double) v)
#define PHPSCITER_ADD_ASSOC_ZVAL_EX(z, s, l, zn) add_assoc_zval_ex(z, s, l, zn)
#define PHPSCITER_ADD_ASSOC_ZVAL_EX_AND(z, s, l, zn) add_assoc_zval_ex(z, s, l, zn)
#define PHPSCITER_ADD_ASSOC_STRING_EX(a, k, l, s) add_assoc_string_ex(a, k, l, s, 1)
#define PHPSCITER_ADD_NEXT_INDEX_STRING(a, s) add_next_index_string(a, s, 1)
#define PHPSCITER_ADD_NEXT_INDEX_STRINGL(a, s, l) add_next_index_stringl(a, s, l, 1)
#define PHPSCITER_ZEND_HASH_GET_CURRENT_KEY(ht, key, idx) zend_hash_get_current_key(ht, key, idx, 0)
#define PHPSCITER_ZEND_HASH_INDEX_UPDATE(ht, h, pData, nDataSize, pDest)  zend_hash_index_update(ht, h, pData, nDataSize, pDest)

#define PHPSCITER_ZEND_UPDATE_PROPERTY(ce,z,zl,zn) zend_update_property(ce, z, zl, zn TSRMLS_CC)
#define PHPSCITER_ZEND_UPDATE_PROPERTY_NULL(ce,z,zl) zend_update_property_null(ce, z, zl TSRMLS_CC)
#define PHPSCITER_ZEND_UPDATE_PROPERTY_EX(ce,z,zl,zn) zend_update_property_ex(ce, z, zl, zn TSRMLS_CC)
#define PHPSCITER_ZEND_UPDATE_STATIC_PROPERTY(ce,zl,zn) zend_update_static_property(ce, zl, zn TSRMLS_CC)
#define PHPSCITER_ZEND_UPDATE_PROPERTY_LONG(ce,z,zl,zn) zend_update_property_long(ce,z,zl,zn TSRMLS_CC)
#define PHPSCITER_ZEND_UPDATE_PROPERTY_STRING(ce,z,zl,s) zend_update_property_string(ce,z,zl,s TSRMLS_CC);
#define PHPSCITER_ZEND_READ_PROPERTY(ce,z,sl)  zend_read_property(ce, z, sl, 1 TSRMLS_CC)

#define PHPSCITER_ZEND_IS_CALLBACK zend_is_callable
#define PHPSCITER_ZEND_IS_CALLBACK_EX zend_is_callable_ex

#define TS_S_L(s) s, strlen(s)

#define PHPSCITER_ZEND_CALL_METHOD_WITH_0_PARAMS zend_call_method_with_0_params
#define PHPSCITER_ZEND_CALL_METHOD_WITH_1_PARAMS zend_call_method_with_1_params
#define PHPSCITER_ZEND_CALL_METHOD_WITH_2_PARAMS zend_call_method_with_2_params

#define PHPSCITER_CALL_USER_FUNCTION_EX call_user_function_ex

#define PHPSCITER_MAKE_STD_ZVAL(p) MAKE_STD_ZVAL(p)
#define PHPSCITER_ALLOC_INIT_ZVAL(p) ALLOC_INIT_ZVAL(p)
#define PHPSCITER_COPY_TO_STACK(a, b)
#define PHPSCITER_ZVAL_PTR_DTOR zval_ptr_dtor
#endif