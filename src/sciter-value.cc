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

zval **CallBackGetValueTmp;

char *getSciterString(const VALUE *p)
{
    char *cstr = NULL;

    aux::wchars wc;
    if(ValueStringData(p, (LPCWSTR*) &wc.start, (UINT*)&wc.length) == HV_OK)
    {
        aux::w2utf str(aux::make_string(wc));
        spprintf(&cstr, 0, "%s",str.c_str());
    }
    else
    {
        aux::w2utf str_null(WSTR(""));
        spprintf(&cstr, 0, "%s",str_null.c_str());
    }

    return cstr;
}

int getSciterInt(const VALUE *p)
{
    INT b;
    ValueIntData(p, &b);

    return (int)b;
}

VALUE NewValue()
{
    VALUE key;
    ValueInit(&key);
    return key;
}

//@TODO Support nested array incoming
BOOL SC_CALLBACK KeyValueCallbackElement(LPVOID param, const VALUE* pkey, const VALUE* pval)
{
    zval zval_key_tmp;
    zval zval_val_tmp;

    switch(pkey->t)
    {
    case T_STRING:
    case T_INT:
        SetPHPValue(pkey, &zval_key_tmp);
        break;
    default:
        return false;
        break;
    }

    switch(pval->t)
    {
    case T_NULL:
    case T_BOOL:
    case T_INT:
    case T_FLOAT:
    case T_STRING:
        SetPHPValue(pval, &zval_val_tmp);
#if PHP_VERSION_ID < 70000
        zval_dtor(&zval_val_tmp);
#endif
        break;
    case T_OBJECT: //TODO Support nested array incoming
    default:
        return true;
        break;
    }

    switch(Z_TYPE_P(&zval_key_tmp))
    {
    case IS_LONG:
        PHPSCITER_ADD_INDEX_ZVAL(*(CallBackGetValueTmp),Z_LVAL_P(&zval_key_tmp),&zval_val_tmp);
        break;
    case IS_STRING:
        PHPSCITER_ADD_ASSOC_ZVAL_EX(*(CallBackGetValueTmp),Z_STRVAL_P(&zval_key_tmp),Z_STRLEN_P(&zval_key_tmp),&zval_val_tmp);
        break;
    default:
        break;
    }
    zval_dtor(&zval_key_tmp);
    return true;
}

UINT SetPHPValue(const VALUE* val, zval *item)
{
    UINT ok = HV_OK;
    switch (val->t)
    {
    case T_NULL:
    {
        ZVAL_NULL(item);
        break;
    }
    case T_BOOL:
    {
        INT check = 0;
        ok = ValueIntData(val, &check);
        if (ok == HV_OK)
        {
            ZVAL_TRUE(item);
        }
        else
        {
            ZVAL_FALSE(item);
        }
        break;
    }
    case T_INT:
    {
        ZVAL_LONG(item,getSciterInt(val));
        convert_to_long(item);
        break;
    }
    case T_FLOAT:
    {
        FLOAT_VALUE data;
        ok = ValueFloatData(val, &data);
        if (ok == HV_OK)
        {
            ZVAL_DOUBLE(item, data);
        }
        else
        {
            ZVAL_DOUBLE(item, 0);
        }
        break;
    }
    case T_STRING:
    {
        char* tmp_str = (char *)getSciterString(val);
        PHPSCITER_ZVAL_STRING(item, tmp_str);
        convert_to_string(item);
        efree(tmp_str);
//        saveLog(Z_STRVAL_P(item));
        break;
    }
    case T_OBJECT:
    {
        int j = 0;
        INT count;
        ValueElementsCount(val,&count);

        if (count > 0)
        {
            array_init(item);

            switch(val->u)
            {
            case UT_OBJECT_ARRAY:
            {
                for (j = 0; j < count; j++)
                {
                    VALUE val_tmp = NewValue();
                    zval zval_val_tmp;
                    ValueNthElementValue(val, j, &val_tmp);
                    SetPHPValue((const VALUE *)&val_tmp,&zval_val_tmp);
                    PHPSCITER_ADD_INDEX_ZVAL(item,j,&zval_val_tmp);
#if PHP_VERSION_ID < 70000
                    zval_dtor(&zval_val_tmp);
#endif
                }

                break;
            }
            case UT_OBJECT_FUNCTION:
            case UT_OBJECT_OBJECT:
            {
                CallBackGetValueTmp = &item;
                ValueEnumElements(val,KeyValueCallbackElement, nullptr);
                break;
            }
            default:
                break;
            }
        }
        break;
    }
    case T_CURRENCY:
    case T_ARRAY:
    case T_MAP:
    case T_FUNCTION:
    default:
        php_printf("Default. Unsupport Data Type -> %d\n",val->t);
        // php_printf("Default. Unsupport Data Type\n");
        break;
    }

    return ok;
}

UINT SetSciterValue(VALUE *result, zval *entry)
{
    UINT ok = 0;
    int type_id = Z_TYPE_P(entry);
//    php_printf("SetSciterValue Z_TYPE_PP(entry) -> %d, IS_DOUBLE -> %d, IS_ARRAY -> %d\n", type_id, IS_DOUBLE, IS_ARRAY);
    switch (type_id)
    {
    case IS_LONG:
    {

        ok = ValueIntDataSet(result,(INT)Z_LVAL_P(entry),T_INT,0);
        break;
    }
    case IS_DOUBLE:
    {
        ok = ValueFloatDataSet(result,(FLOAT_VALUE)Z_DVAL_P(entry),T_FLOAT,0);
        break;
    }
    case IS_STRING:
    {
        const char *str = Z_STRVAL_P(entry);
        int len = Z_STRLEN_P(entry);
        int length = 0;

        aux::utf2w entry_as_utf8_wstr(str);
        length = UINT(entry_as_utf8_wstr.length());

        if (len != length)
        {
            ok = ValueStringDataSet(result,LPCWSTR(entry_as_utf8_wstr.c_str()),length,UT_STRING_STRING);
        }
        else
        {
            aux::a2w entry_as_wstr(str);
            length = UINT(entry_as_wstr.length());
            ok = ValueStringDataSet(result,LPCWSTR(entry_as_wstr.c_str()),length,UT_STRING_STRING);
        }
        break;
    }
    case IS_NULL:
    {
        ok = ValueIntDataSet(result,(INT)0,T_NULL,0);
        break;
    }
#if PHP_VERSION_ID >= 70000
    case IS_FALSE:
    {
        ok = ValueIntDataSet(result,(INT)0,T_BOOL,0);
        break;
    }
    case IS_TRUE:
    {
        ok = ValueIntDataSet(result,(INT)1,T_BOOL,0);
        break;
    }
#else
    case IS_BOOL:
    {
        long hval = (Z_LVAL_P(entry)?1:0);
        ok = ValueIntDataSet(result,(INT)hval,T_BOOL,0);
        break;
    }
#endif
    case IS_ARRAY:
    {
        ValueIntDataSet(result,(INT)0, T_MAP, 0);

        HashTable   *ht;
        zend_ulong num_key;
        zend_string *str_key;

        ht = Z_ARRVAL_P(entry);
        zend_long num_in = PHPSCITER_ARRAY_COUNT(ht);

        if (num_in > 0)
        {
#if PHP_VERSION_ID >= 70000
        zval *entry_tmp;
        ZEND_HASH_FOREACH_KEY_VAL(ht, num_key, str_key, entry_tmp)
        {
            VALUE key = NewValue();
            if (str_key)
            {
                aux::a2w str_key_as_wstr(PHPSCITER_ZSTR_VAL(str_key));
                ValueStringDataSet(&key,LPCWSTR(str_key_as_wstr.c_str()), UINT(str_key_as_wstr.length()), T_STRING);
            }
            else
            {
                ValueIntDataSet(&key,(INT)num_key,T_INT,0);
            }

            VALUE val = NewValue();
            ok = SetSciterValue(&val, entry_tmp);

            ValueSetValueToKey(result,&key,&val);
        }
        ZEND_HASH_FOREACH_END();
#else
        HashPosition iterator;
        zval **tmp;
        uint str_len;
        zend_hash_internal_pointer_reset_ex(ht, &iterator);
        while (zend_hash_get_current_data_ex(ht, (void **) &tmp, &iterator) == SUCCESS) {
            VALUE key = NewValue();
            VALUE val = NewValue();
            str_key = nullptr;
            zend_hash_get_current_key_ex(ht, &str_key, &str_len, &num_key, 0, &iterator);
            if(str_key)
            {
                aux::a2w str_key_as_wstr(PHPSCITER_ZSTR_VAL(str_key));
                ValueStringDataSet(&key,LPCWSTR(str_key_as_wstr.c_str()), UINT(str_key_as_wstr.length()), T_STRING);
            }else{
                ValueIntDataSet(&key,(INT)num_key,T_INT,0);
            }

            ok = SetSciterValue(&val, *tmp);
            ValueSetValueToKey(result,&key,&val);
            zend_hash_move_forward_ex(ht, &iterator);
        }
#endif
        }
        break;
    }
    default:
    {
        php_printf("default Z_TYPE_P(entry) -> %d\n", type_id);
        break;
    }
    }

    return ok;
}



