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
#include <include/sciter-x.h>
#include "php_sciter.h"
#include <string>
using namespace std;
static HashTable callbacks;

BOOL initFunctions()
{
    zend_hash_init(&callbacks, 0, NULL, NULL, 1);

    return true;
}

BOOL functionRegister(zend_string *event_name, zval *callback)
{
    zend_hash_add(&callbacks,event_name,callback);
    return true;
}

BOOL checkRegisted(zend_string *event_name)
{
    if (zend_hash_find(&callbacks, event_name))
    {
        return true;
    }

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
            if(wu.like(WSTR("file:///*")) && (wu.like(WSTR("file:///*.php")) || wu.like(WSTR("file:///*.php?*")))) {
                std::string resource_dir = PHPSCITER_G(tool)->U16toString(pc->uri);
                char* file_name = (char*)resource_dir.c_str() + 7;
                PHPSCITER_G(request)->initRequest(file_name);
                PHPSCITER_G(request)->onRequest(pc);
                resource_dir = PHPSCITER_G(request)->onComplete();
                PHPSCITER_G(request)->onClose();
                file_name = (char *)resource_dir.c_str();

                if(PHPSCITER_G(tool)->isFile(file_name) == FAILURE) {
                    return LOAD_OK;
                }

                zval content;
                zval args[1];//自定义参数
                zval zend_file_name;
                ZVAL_STRING(&zend_file_name,file_name);
                args[0] = zend_file_name;
                call_user_function_ex(EG(function_table),&EG(current_execute_data)->This, PHPSCITER_G(load_hook_name),
                                      &content, 1, args, 0, nullptr);

                if(Z_TYPE(content) == IS_STRING)
                {
//                        aux::a2w w_content(Z_STRVAL(content));
                        ::SciterDataReady(pc->hwnd, pc->uri, (LPCBYTE) (Z_STRVAL(content)), Z_STRLEN(content));
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

/**
*Sciter and php function proxy
*/
BOOL SciterExecuteFunction(HELEMENT he, SCRIPTING_METHOD_PARAMS* p)
{
    zend_string *event_name;
    event_name = TS_S_L((char *)(p->name));
    zval *callback = zend_hash_find(&callbacks, event_name);
    int args_count = 0;

    // php_printf("executeFunciton event_name->val -> %s, event_name->len -> %d\n",event_name->val,event_name->len);

    if (callback)
    {
        if (IS_STRING == Z_TYPE_P(callback))
        {
            args_count = p->argc;
            zval *args = new zval[args_count];
            zval *retval = NULL;
            zval retval_copy;

            const VALUE* p2;
            UINT ok;
            int i = 0;
            for (i = 0; i < (int)p->argc; i++)
            {
                p2 = p->argv + i;

                ok = SetPHPValue(p2, &args[i]);
            }

            if (PHPSCITER_CALL_USER_FUNCTION_EX(EG(function_table), NULL, callback, &retval, p->argc, args, 0, NULL TSRMLS_CC) == FAILURE)
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
                ZVAL_COPY(&retval_copy,retval);
                ok = SetSciterValue(&p->result, &retval_copy);
            }

            return true;
        }
        else
        {
            php_printf("find callback faild -> %s\n",event_name->val);
            return false;
        }
    }
    else
    {
        php_printf("find callback faild -> %s\n",event_name->val);
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


