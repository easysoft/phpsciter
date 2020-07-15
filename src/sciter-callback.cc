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
using namespace std;
static HashTable callbacks;

BOOL initFunctions()
{
    zend_hash_init(&callbacks, 0, NULL, NULL, 1);

    return true;
}

BOOL functionRegister(zend_string *event_name, zval *callback)
{
#if PHP_VERSION_ID >= 70000
    if(!zend_hash_add(&callbacks,event_name,callback))
    {
        return  false;
    }
#else
    int res;
    res = zend_hash_add(&callbacks, event_name, strlen(event_name)+1,
            &callback, sizeof(zval), nullptr);
    if(res == FAILURE)
    {
        return false;
    }
#endif
    return true;
}

BOOL checkRegisted(zend_string *event_name)
{
#if PHP_VERSION_ID >= 70000
    if (zend_hash_find(&callbacks, event_name))
    {
        return true;
    }
#else
    zval** find_unit = nullptr;
    if ((zend_hash_find(&callbacks, event_name, (uint)(strlen(event_name)+1), (void**)&find_unit)) == SUCCESS)
    {
        return true;
    }
#endif

    return false;
}
HINSTANCE ghInstance = 0;

// handle SC_LOAD_DATA requests - get data from resources of this application
UINT SC_CALLBACK SciterViewCallback(LPSCITER_CALLBACK_NOTIFICATION pns, LPVOID callbackParam )
{
    BOOL res;
    switch(pns->code)
    {
        case SC_LOAD_DATA:
        {
            LPSCN_LOAD_DATA pc = LPSCN_LOAD_DATA(pns);
            aux::wchars wu = aux::chars_of(pc->uri);

            if(wu.like(WSTR("file://*")) && (wu.like(WSTR("file://*.php")) || wu.like(WSTR("file://*.php?*")))) {
                std::string resource_dir = PHPSCITER_G(tool)->U16toString(pc->uri);
                char* file_name = (char*)resource_dir.c_str() + 7;
                bool ret = PHPSCITER_G(zend)->zendExecuteScript(file_name, pc);
                if(ret)
                {
                    ::SciterDataReady(pc->hwnd, pc->uri, (LPCBYTE) (PHPSCITER_G(output_buffer).c_str()),
                                      PHPSCITER_G(output_buffer).length());
                    if(!PHPSCITER_G(output_buffer).empty())
                    {
                        PHPSCITER_G(output_buffer).clear();
                    }
                }
            }else {
                return LOAD_OK;
            }

            return LOAD_OK;
        }
        case SC_DATA_LOADED: {
        }break;
        case SC_ATTACH_BEHAVIOR: {
        }break;
    }
    // php_printf("SciterViewCallback end\n");
    return 0;
}

struct ArgsGuard{
public:
#if PHP_VERSION_ID >= 70000
    ArgsGuard(SCRIPTING_METHOD_PARAMS* params, zval** args)
    {
        int i = 0;
        const VALUE* p2;
        UINT ok;
        sciter_params = params;
        zval* storage = (zval*)ecalloc(params->argc, sizeof(zval));
        for (i = 0; i < (int)sciter_params->argc; i++)
        {
            p2 = sciter_params->argv + i;
            ok = SetPHPValue(p2, &(storage[i]));
        }
        zval_args = storage;
        *args = storage;
    }

    ~ArgsGuard()
    {
        int i = 0;
        zval* tmp;
        for (i = 0; i < (int)sciter_params->argc; i++)
        {
            tmp = &zval_args[i];
            zval_dtor(tmp);
        }
        efree(zval_args);
    }

private:
    SCRIPTING_METHOD_PARAMS* sciter_params;
    zval* zval_args;
#else
    ArgsGuard(SCRIPTING_METHOD_PARAMS* params, zval*** &args)
    {
        int i = 0;
        const VALUE* p2;
        UINT ok;
        sciter_params = params;
        ALLOC_CALLBACK_ARGS(args, 0, (int)sciter_params->argc)
        for (i = 0; i < (int)sciter_params->argc; i++)
        {
            p2 = sciter_params->argv + i;
            ok = SetPHPValue(p2, (*args[i]));
        }
        zval_args = args;
    }

    ~ArgsGuard()
    {
        int i = 0;
        FREE_CALLBACK_ARGS(zval_args, 0, sciter_params->argc);
    }

    private:
    SCRIPTING_METHOD_PARAMS* sciter_params;
    zval*** zval_args;
#endif
};

/**
*Sciter and php function proxy
*/
BOOL SciterExecuteFunction(HELEMENT he, SCRIPTING_METHOD_PARAMS* p)
{
    zend_string *event_name;
    event_name = TS_S_L((char *)(p->name));
#if PHP_VERSION_ID >= 70000
    zval *callback = zend_hash_find(&callbacks, event_name);
#else
    zval** callback;
    int res = zend_hash_find(&callbacks, event_name, strlen(event_name)+1,(void**)&callback);
    if(res == FAILURE)
        return  false;
#endif
    int args_count = 0;

    // php_printf("executeFunciton event_name->val -> %s, event_name->len -> %d\n",event_name->val,event_name->len);

    if (callback)
    {
#if PHP_VERSION_ID >= 70000
        if (IS_STRING == Z_TYPE_P(callback))
#else
        if (IS_STRING == Z_TYPE_P(*callback))
#endif
        {

#if PHP_VERSION_ID >= 70000
            args_count = p->argc;
            zval* args;
            zval retval;
            zval retval_copy;

            UINT ok;
            ArgsGuard guard(p ,&args);

            if (PHPSCITER_CALL_USER_FUNCTION_EX(EG(function_table), nullptr, callback, &retval, p->argc, args,
                    0, nullptr TSRMLS_CC) == FAILURE)
            {
                php_printf("executeFunction error -> \n");
                zval_dtor(&retval);
                zend_string_release(event_name);
                return false;
            }

            if (EG(exception))
            {
                zend_exception_error(EG(exception), E_ERROR TSRMLS_CC);
            }


            ZVAL_COPY(&retval_copy,&retval);
            ok = SetSciterValue(&p->result, &retval_copy);
            zval_dtor(&retval);
            zval_dtor(&retval_copy);
            zend_string_release(event_name);
            return  true;
#else
            zval ***args = nullptr;
            ArgsGuard guard(p, args);
            zval *retval = nullptr;
            zval* retval_copy;
            zval* zv;
            zval *unit;
            UINT ok;

            if (PHPSCITER_CALL_USER_FUNCTION_EX(EG(function_table), nullptr, *callback, &retval, p->argc, args, 0,
                                                nullptr TSRMLS_CC) == FAILURE)
            {
                php_printf("executeFunction error -> \n");

                return false;
            }

            if (EG(exception))
            {
                zend_exception_error(EG(exception), E_ERROR TSRMLS_CC);
            }

            if (retval != NULL)
            {
                MAKE_STD_ZVAL(retval_copy);
                *retval_copy = *retval;
                zval_copy_ctor(retval_copy);
                ok = SetSciterValue(&p->result, retval_copy);
                zval_ptr_dtor(&retval_copy);
                zval_ptr_dtor(&retval);
            }
#endif
            return true;
        }
        else
        {
#if PHP_VERSION_ID > 70000
            php_printf("find callback faild -> %s\n",event_name->val);
            zend_string_release(event_name);
#else
            php_printf("find callback faild -> %s\n",event_name);
#endif
            return false;
        }
    }
    else
    {
#if PHP_VERSION_ID >= 70000
        php_printf("find callback faild -> %s\n",event_name->val);
        zend_string_release(event_name);
#else
        php_printf("find callback faild -> %s\n",event_name);
#endif
        return false;
    }

    return true;
}

BOOL SC_CALLBACK  ElementEventProcImplementeation(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms)
{
    // HELEMENT *pThis = Sciter_UseElement((HELEMENT *)he);
    // Sciter_UseElement(he);
    // if(he) {
    switch(evtg)
    {
    // case SUBSCRIPTIONS_REQUEST:
    //     {
    //         UINT *p = (UINT *)prms;
    //         php_printf("my_element_proc SUBSCRIPTIONS_REQUEST \n");
    //         return true;
    //         // return *pThis->subscription( he, *p );
    //     }
    // case HANDLE_INITIALIZATION:
    //     {
    //         INITIALIZATION_PARAMS *p = (INITIALIZATION_PARAMS *)prms;
    //         if(p->cmd == BEHAVIOR_DETACH) {
    //             // pThis.detached(he);
    //         }else if(p->cmd == BEHAVIOR_ATTACH) {
    //             // pThis.attached(he);
    //         }
    //         php_printf("my_element_proc HANDLE_INITIALIZATION \n");
    //         return true;
    //     }
    // case HANDLE_MOUSE:
    //     {
    //         return false;
    //         MOUSE_PARAMS *p = (MOUSE_PARAMS *)prms;
    //         php_printf("my_element_proc HANDLE_MOUSE \n");
    //         return true;
    //         // return pThis.handle_mouse( he, *p );
    //     }
    // case HANDLE_KEY:
    //     {
    //         KEY_PARAMS *p = (KEY_PARAMS *)prms;
    //         php_printf("my_element_proc HANDLE_KEY \n");
    //         return true;
    //         // return pThis.handle_key( he, *p );
    //     }
    // case HANDLE_FOCUS:
    //     {
    //         return false;

    //         FOCUS_PARAMS *p = (FOCUS_PARAMS *)prms;
    //         php_printf("my_element_proc HANDLE_FOCUS \n");
    //         return true;
    //         //return pThis.handle_focus( he, *p );
    //     }
    // case HANDLE_DRAW:
    //     {
    //         DRAW_PARAMS *p = (DRAW_PARAMS *)prms;
    //         php_printf("my_element_proc HANDLE_DRAW \n");
    //         return true;
    //         //return pThis.handle_draw(he, *p );
    //     }
    // case HANDLE_TIMER:
    //     {
    //         TIMER_PARAMS *p = (TIMER_PARAMS *)prms;
    //         php_printf("my_element_proc HANDLE_TIMER \n");
    //         return true;
    //         //return pThis.handle_timer(he, *p);
    //     }
    // case HANDLE_BEHAVIOR_EVENT:
    //     {
    //         BEHAVIOR_EVENT_PARAMS *p = (BEHAVIOR_EVENT_PARAMS *)prms;
    //         php_printf("my_element_proc HANDLE_BEHAVIOR_EVENT \n");
    //         php_printf("p->cmd -> %d \n",p->cmd);

    //         return true;
    //         //return pThis.handle_event(he, *p );
    //     }
    // case HANDLE_METHOD_CALL:
    //     {
    //         METHOD_PARAMS *p = (METHOD_PARAMS *)prms;
    //         php_printf("my_element_proc HANDLE_METHOD_CALL \n");
    //         return true;
    //         //return pThis.handle_method_call(he, *p );
    //     }
    // case HANDLE_DATA_ARRIVED:
    //     {
    //         DATA_ARRIVED_PARAMS *p = (DATA_ARRIVED_PARAMS *)prms;
    //         php_printf("my_element_proc HANDLE_DATA_ARRIVED \n");
    //         return true;
    //         //return pThis.handle_data_arrived(he, *p );
    //     }
    // case HANDLE_SCROLL:
    //     {
    //         SCROLL_PARAMS *p = (SCROLL_PARAMS *)prms;
    //         php_printf("my_element_proc HANDLE_SCROLL \n");
    //         return true;
    //         //return pThis.handle_scroll(he, *p );
    //     }
    // case HANDLE_SIZE:
    //     {
    //         php_printf("my_element_proc HANDLE_SIZE \n");
    //         return true;
    //         //pThis.handle_size(he);
    //         //return false;
    //     }
    // call using sciter::value's (from CSSS!)
    case HANDLE_SCRIPTING_METHOD_CALL:
    {
        SCRIPTING_METHOD_PARAMS* p = (SCRIPTING_METHOD_PARAMS *)prms;
        return SciterExecuteFunction(he,p);
    }
    // call using tiscript::value's (from the script)
    // case HANDLE_TISCRIPT_METHOD_CALL:
    //     {
    //         php_printf("my_element_proc HANDLE_TISCRIPT_METHOD_CALL 222222\n");
    //         // TISCRIPT_METHOD_PARAMS* p = (TISCRIPT_METHOD_PARAMS *)prms;
    //         SCRIPTING_METHOD_PARAMS* p = (SCRIPTING_METHOD_PARAMS *)prms;
    //         php_printf("p->name => %s\n",p->name);
    //         if (strcmp(p->name,"getNetInformation") == 0) {
    //             php_printf("p->name => %s\n",p->name);
    //             php_printf("p->argc => %d\n",p->argc);
    //             php_printf("p->argv[1] => %s\n",p->argv[1]);
    //             php_printf("p->result => %s\n",p->result);
    //         }
    //         return true;
    //         //return pThis.handle_scripting_call(he, *p );
    //     }
    // case HANDLE_GESTURE:
    //     {
    //         GESTURE_PARAMS *p = (GESTURE_PARAMS *)prms;
    //         php_printf("my_element_proc HANDLE_GESTURE \n");
    //         return true;
    //         //return pThis.handle_gesture(he, *p );
    //     }
    default:
        return false;
    }
    // }

    return false;
}


BOOL clearCallBack()
{
#if PHP_VERSION_ID >= 70000
    zend_symtable_clean(&callbacks);
#else
    HashPosition iterator;
    zval **tmp;
    zend_hash_internal_pointer_end_ex(&callbacks, &iterator);
    while (zend_hash_get_current_data_ex(&callbacks, (void **) &tmp, &iterator) == SUCCESS) {
        if(!iterator->arKey)
        {
            continue;
        }
        PHPSCITER_ZVAL_DTOR(*tmp);
        zend_hash_move_backwards_ex(&callbacks, &iterator);
    }
#endif
    return TRUE;
}