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
#define PHPSCITER_RETURN_STRING(k) RETURN_STRING(k)
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

//zval
#define PHPSCITER_ZVAL_STR(z, s) ZVAL_STR(z, s)
#define PHPSCITER_ZVAL_COPY(z, d) ZVAL_COPY(z, d)

//zend_String
#define PHPSCITER_ZSTR_VAL(zstr) ZSTR_VAL(zstr)

//array
#define PHPSCITER_ARRAY_COUNT(ht) zend_array_count(ht)

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

#define PHPSCITER_MAKE_STD_ZVAL(p) zval _stack_zval_##p; p = &(_stack_zval_##p)
#define PHPSCITER_ALLOC_INIT_ZVAL(p) do{p = (zval *)emalloc(sizeof(zval)); bzero(p, sizeof(zval));}while(0)
#define PHPSCITER_COPY_TO_STACK(a, b) {zval *__tmp = a; a = &b; memcpy(a, __tmp, sizeof(zval));}
#define PHPSCITER_ZVAL_PTR_DTOR(p)  zval_ptr_dtor(*p)
#define PHPSCITER_ZVAL_DTOR(p) zval_ptr_dtor(p)

#define ZEND_GET "_GET"
#define ZEND_GET_LEN sizeof(ZEND_GET)-1
#define ZEND_POST "_POST"
#define ZEND_POST_LEN sizeof(ZEND_POST)-1
#define ZEND_REQUEST "_REQUEST"
#define ZEND_REQUEST_LEN sizeof(ZEND_REQUEST)-1
#define ZEND_SERVER "_SERVER"
#define ZEND_SERVER_LEN sizeof(ZEND_SERVER)-1
#define ZEND_SERVER_REQUEST_URI "REQUEST_URI"
#define ZEND_SERVER_REQUEST_URI_LEN sizeof(ZEND_SERVER_REQUEST_URI)-1
#define ZEND_COOKIE "_COOKIE"
#define ZEND_COOKIE_LEN sizeof(ZEND_COOKIE)-1

static inline int PHPSCITER_CALL_USER_FUNCTION_EX(HashTable *function_table, zval** object_pp, zval *function_name, zval *retval_ptr_ptr, uint32_t param_count, zval *params, int no_separation, HashTable* ymbol_table)
{
    zval real_params[PHPSCITER_PHP_MAX_PARAMS_NUM];
    int i = 0;
    for (; i < (int)param_count; i++)
    {
        real_params[i] = params[i];
    }
    zval *object_p = (object_pp == NULL) ? NULL : *object_pp;
    return call_user_function_ex(function_table, object_p, function_name, retval_ptr_ptr, param_count, params, no_separation, NULL);
}
#else

#define zend_string char

#define PHPSCITER_STR_SIZE(s) strlen(s) + 1
#define PHPSCITER_ZVAL_STRING(z, s) ZVAL_STRING(z, s, 1)
#define PHPSCITER_ZVAL_STRINGL(z, s, l) ZVAL_STRINGL(z, s, l, 1)
#define PHPSCITER_RETURN_STRING(k) RETURN_STRING(k, 1)
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

//zval
#define PHPSCITER_ZVAL_STR(z, s) PHPSCITER_ZVAL_STRING(z, s)
#define PHPSCITER_ZVAL_COPY(z, d) ZVAL_COPY(z, d)
#define PHPSCITER_ZVAL_DTOR(p) zval_ptr_dtor(&p)

//zend_string
#define PHPSCITER_ZSTR_VAL(zstr) (zstr)
#define PHPSCITER_ZSTR_LEN(zstr) strlen(zstr)

//array
#define PHPSCITER_ARRAY_COUNT(ht) zend_hash_num_elements(ht)

//zend_int
/* Integer types. */
#ifdef ZEND_ENABLE_ZVAL_LONG64
typedef int64_t zend_long;
typedef uint64_t zend_ulong;
typedef int64_t zend_off_t;
# define ZEND_LONG_MAX INT64_MAX
# define ZEND_LONG_MIN INT64_MIN
# define ZEND_ULONG_MAX UINT64_MAX
# define Z_L(i) INT64_C(i)
# define Z_UL(i) UINT64_C(i)
# define SIZEOF_ZEND_LONG 8
#else
typedef int32_t zend_long;
typedef int32_t zend_off_t;
#ifndef ZEND_LONG_MAX
# define ZEND_LONG_MAX INT32_MAX
#endif
#ifndef ZEND_LONG_MIN
# define ZEND_LONG_MIN INT32_MIN
#endif
#ifndef ZEND_ULONG_MAX
# define ZEND_ULONG_MAX UINT32_MAX
#endif
#ifndef Z_L(i)
# define Z_L(i) INT32_C(i)
#endif
#ifndef Z_UL(i)
# define Z_UL(i) UINT32_C(i)
#endif
#ifndef SIZEOF_ZEND_LONG
# define SIZEOF_ZEND_LONG 4
#endif
#endif

#ifndef ZVAL_COPY(d, z)
#define ZVAL_COPY(d, z) zval_copy_ctor(&d);
#endif

#define ZEND_GET "_GET"
#define ZEND_GET_LEN strlen(ZEND_GET)
#define ZEND_POST "_POST"
#define ZEND_POST_LEN strlen(ZEND_POST)
#define ZEND_REQUEST "_REQUEST"
#define ZEND_REQUEST_LEN strlen(ZEND_REQUEST)
#define ZEND_SERVER "_SERVER"
#define ZEND_SERVER_LEN strlen(ZEND_SERVER)
#define ZEND_SERVER_REQUEST_URI "REQUEST_URI"
#define ZEND_SERVER_REQUEST_URI_LEN sizeof(ZEND_SERVER_REQUEST_URI)
#define ZEND_COOKIE "_COOKIE"
#define ZEND_COOKIE_LEN strlen(ZEND_COOKIE)

#ifndef ALLOC_CALLBACK_ARGS
#define ALLOC_CALLBACK_ARGS(a, b, c)\
if (c) {\
	a = (zval ***)safe_emalloc(c, sizeof(zval **), 0);\
	for (i = b; i < c; i++) {\
		a[i] = (zval**)emalloc(sizeof(zval *));\
		MAKE_STD_ZVAL(*a[i]);\
	}\
}
#endif

#ifndef FREE_CALLBACK_ARGS
#define FREE_CALLBACK_ARGS(a, b, c)\
if (a) {\
	for (i=b; i < c; i++) {\
		zval_ptr_dtor(a[i]);\
		efree(a[i]);\
	}\
	efree(a);\
}
#endif

#endif