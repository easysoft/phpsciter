//
// Created by root on 20-3-7.
//

#include "phpsciter_application.h"
#include "php_sciter.h"

zend_class_entry *phpsciter_ce,*php_com_exception_class_entry;

const zend_function_entry phpsciter_methods[] =
{
        PHP_ME(phpsciter, __construct,  NULL, ZEND_ACC_PRIVATE | ZEND_ACC_CTOR)
        PHP_ME(phpsciter, __destruct,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
        PHP_ME(phpsciter, __clone,      NULL, ZEND_ACC_PRIVATE)
        PHP_ME(phpsciter, __sleep,      NULL, ZEND_ACC_PRIVATE)
        PHP_ME(phpsciter, __wakeup,     NULL, ZEND_ACC_PRIVATE)

        PHP_ME(phpsciter, getInstance,  NULL, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        PHP_ME(phpsciter, run, 			NULL, ZEND_ACC_PUBLIC)

        PHP_ME(phpsciter, getVersion,   NULL, ZEND_ACC_PUBLIC)
        PHP_ME(phpsciter, getPHPSciterVersion,   NULL, ZEND_ACC_PUBLIC)
        PHP_ME(phpsciter, setResourcePath, phpsciter_setResourcePath_arginfo, ZEND_ACC_PUBLIC)
        PHP_ME(phpsciter, setWindowFrame, phpsciter_setWindowFrame_arginfo, ZEND_ACC_PUBLIC)
        PHP_ME(phpsciter, setWindowTitle, phpsciter_setWindowTitle_arginfo, ZEND_ACC_PUBLIC)
        PHP_ME(phpsciter, loadFile, phpsciter_loadFile_arginfo, ZEND_ACC_PUBLIC)
        PHP_ME(phpsciter, loadPHP, phpsciter_loadPHP_arginfo, ZEND_ACC_PUBLIC)
        PHP_ME(phpsciter, loadHtml, phpsciter_loadHtml_arginfo, ZEND_ACC_PUBLIC)

        PHP_ME(phpsciter, defineFunction, phpsciter_defineFunction_arginfo, ZEND_ACC_PUBLIC)
        PHP_ME(phpsciter, ifDefined, phpsciter_ifDefined_arginfo, ZEND_ACC_PUBLIC)

        {
            NULL, NULL, NULL
        }
};

PHP_METHOD(phpsciter, __construct)
{
}

PHP_METHOD(phpsciter, __destruct)
{
}

PHP_METHOD(phpsciter, __sleep)
{
}

PHP_METHOD(phpsciter, __wakeup)
{
}

PHP_METHOD(phpsciter, __clone)
{
}

PHP_METHOD(phpsciter, getInstance)
{
        int argc = ZEND_NUM_ARGS();
        char *instance_name;
        zval *get_instance_array = NULL;

#if PHP_VERSION_ID >= 70000
        size_t  instance_name_len;
        zval *ppzval = NULL;
        zval set_instance_array;
#else
        int  instance_name_len;
    zval **ppzval = NULL;
    zval *set_instance_array;
#endif

        zval *instance;

//In php7 , this params instance_name can not be empty.
#if PHP_VERSION_ID >= 70000
        if (zend_parse_parameters(argc TSRMLS_CC, "s", &instance_name, &instance_name_len) == FAILURE)
        {
                zend_throw_exception(php_com_exception_class_entry,"The instance_name can not be empty,you can use phpsciter::PHPSCITER_INSTANCE_DEFAULT",999 TSRMLS_CC);
                RETURN_FALSE;
        }
#else
        if (zend_parse_parameters(argc TSRMLS_CC, "|s", &instance_name, &instance_name_len) == FAILURE)
    {
        RETURN_FALSE;
    }
#endif

        if (argc < 1)
        {
                instance_name     = PHPSCITER_INSTANCE_DEFAULT;
                instance_name_len = PHPSCITER_INSTANCE_DEFAULT_LEN;
        }

        get_instance_array = zend_read_static_property(phpsciter_ce, ZEND_STRL(PHPSCITER_NAME), 1 TSRMLS_CC);

        if (get_instance_array && Z_TYPE_P(get_instance_array) == IS_ARRAY)
        {

#if PHP_VERSION_ID >= 70000
                if ((ppzval = zend_hash_str_find(Z_ARRVAL_P(get_instance_array),instance_name,instance_name_len)) != NULL)
                {
                        RETURN_ZVAL(ppzval, 1, 0);
                }
                else
                {
                        goto initInstance;
                }
#else
                if (zend_hash_find(Z_ARRVAL_P(get_instance_array), ZEND_STRL(instance_name), (void **)&ppzval) == SUCCESS )
        {
            RETURN_ZVAL(*ppzval, 1, 0);
        }
        else
        {
            goto initInstance;
        }
#endif
        }
        else
        {
#if PHP_VERSION_ID >= 70000
                array_init(&set_instance_array);
#else
                MAKE_STD_ZVAL(set_instance_array);
        array_init(set_instance_array);
#endif
                goto initInstance;
        }

    initInstance:
#if PHP_VERSION_ID >= 70000
        instance = getThis();
        zval re_instance;

        if (!instance)
        {
                ZVAL_NULL(&re_instance);
                instance = &re_instance;
        }
#else
        MAKE_STD_ZVAL(instance);
#endif

        object_init_ex(instance, phpsciter_ce);

        PHPSCITER_ZEND_UPDATE_PROPERTY_STRING(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_RESOURCE_PATH), PHPSCITER_G(resource_base_path));
        PHPSCITER_ZEND_UPDATE_PROPERTY_STRING(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_TITLE), PHPSCITER_G(default_title));

        PHPSCITER_ZEND_UPDATE_PROPERTY_NULL(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_LOAD_FILE));
        PHPSCITER_ZEND_UPDATE_PROPERTY_NULL(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_LOAD_HTML));

        PHPSCITER_ZEND_UPDATE_PROPERTY_LONG(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_TOP), 0);
        PHPSCITER_ZEND_UPDATE_PROPERTY_LONG(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_LEFT), 0);
        PHPSCITER_ZEND_UPDATE_PROPERTY_LONG(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_RIGHT), 0);
        PHPSCITER_ZEND_UPDATE_PROPERTY_LONG(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_BOTTOM), 0);

        if (get_instance_array && IS_ARRAY == Z_TYPE_P(get_instance_array))
        {
                PHPSCITER_ADD_ASSOC_ZVAL_EX(get_instance_array,instance_name,instance_name_len,instance);
                zend_update_static_property(phpsciter_ce, ZEND_STRL(PHPSCITER_NAME), get_instance_array TSRMLS_CC);
        }
        else
        {
                PHPSCITER_ADD_ASSOC_ZVAL_EX_AND(set_instance_array,instance_name,instance_name_len,instance);
                PHPSCITER_ZEND_UPDATE_STATIC_PROPERTY(phpsciter_ce, ZEND_STRL(PHPSCITER_NAME), set_instance_array);

                zval_ptr_dtor(&set_instance_array);
        }

        RETURN_ZVAL(instance, 1, 0);
}

PHP_METHOD(phpsciter, defineFunction)
{
        zval *callback;
        zend_string *event_name;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "Sz", &event_name, &callback) == FAILURE)
        {
                return;
        }

        char *func_name = NULL;
        if (!PHPSCITER_ZEND_IS_CALLBACK(callback, 0, &func_name TSRMLS_CC))
        {
                php_printf("Function '%s' is not callable\n", func_name);
                efree(func_name);
                RETURN_FALSE;
        }

        zval *function_name;
        PHPSCITER_MAKE_STD_ZVAL(function_name);
        ZVAL_STRINGL(function_name , Z_STRVAL_P(callback), Z_STRLEN_P(callback));
        functionRegister(event_name, function_name);

        efree(func_name);

        RETURN_TRUE;
}

PHP_METHOD(phpsciter, ifDefined)
{
        zend_string *event_name;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &event_name) == FAILURE)
        {
                return;
        }

        if (SCDOM_OK == checkRegisted(event_name))
        {
                RETURN_TRUE;
        }

        RETURN_FALSE;
}

PHP_METHOD(phpsciter, run)
{
        zval *instance;
        zval *frame_top,*frame_left,*frame_right,*frame_bottom;
        zval *title,*loadFile,*loadHtml,*resource_path;

        char* file_name = NULL;
        int file_name_len = 0;
        int res;

//        if (!PHPSCITER_G(loadHtml) && !PHPSCITER_G(loadFile))
//        {
//                RETURN_FALSE;
//        }

        instance = getThis();

        frame_top = PHPSCITER_ZEND_READ_PROPERTY(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_TOP));
        frame_left = PHPSCITER_ZEND_READ_PROPERTY(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_LEFT));
        frame_right = PHPSCITER_ZEND_READ_PROPERTY(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_RIGHT));
        frame_bottom = PHPSCITER_ZEND_READ_PROPERTY(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_BOTTOM));

        title = PHPSCITER_ZEND_READ_PROPERTY(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_TITLE));
        resource_path = PHPSCITER_ZEND_READ_PROPERTY(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_RESOURCE_PATH));

        RECT frame;
        frame.top = Z_LVAL_P(frame_top);
        frame.left = Z_LVAL_P(frame_left);
        frame.right = Z_LVAL_P(frame_right);
        frame.bottom = Z_LVAL_P(frame_bottom);

        SciterWindowInit();

        HWINDOW hw = SciterCreateWindow(SW_RESIZEABLE | SW_MAIN | SW_TOOL | SW_ENABLE_DEBUG, &frame, 0,0,0);

        SciterSetCallback(hw, &SciterViewCallback, NULL);
        SciterSetWindowTitle(hw,Z_STRVAL_P(title));

        aux::a2w resource_path_as_wstr(Z_STRVAL_P(resource_path));
        SciterSetHomeURL(hw,LPCWSTR(resource_path_as_wstr.c_str()));

        switch (PHPSCITER_G(loadModal))
        {
            case LOAD_HTML: {
                loadHtml = PHPSCITER_ZEND_READ_PROPERTY(phpsciter_ce, instance,
                                                        ZEND_STRL(PHPSCITER_PROPERTY_LOAD_HTML));
                SciterLoadHtml(hw, (byte *) Z_STRVAL_P(loadHtml), Z_STRLEN_P(loadHtml),
                               LPCWSTR(resource_path_as_wstr.c_str()));
            } break;
            case LOAD_HTML_FILE: {
                loadFile = PHPSCITER_ZEND_READ_PROPERTY(phpsciter_ce, instance,
                                                        ZEND_STRL(PHPSCITER_PROPERTY_LOAD_FILE));

                file_name_len = spprintf(&file_name, 0, "%s%s", Z_STRVAL_P(resource_path), Z_STRVAL_P(loadFile));

                aux::a2w file_name_as_wstr(file_name);
                SciterLoadFile(hw, LPCWSTR(file_name_as_wstr.c_str()));
            } break;
            case LOAD_PHP: {
                string content = PHPSCITER_G(tool)->zendExecute(PHPSCITER_G(cureent_op_array));
                SciterLoadHtml(hw, (byte *) content.c_str(), strlen(content.c_str()),
                               LPCWSTR(resource_path_as_wstr.c_str()));
                efree(PHPSCITER_G(cureent_op_array));
                PHPSCITER_G(cureent_op_array) = nullptr;
            } break;
            case LOAD_PHP_FILE: {
                string content = PHPSCITER_G(tool)->zendExecute(PHPSCITER_G(cureent_op_array));
                SciterLoadHtml(hw, (byte *) content.c_str(), strlen(content.c_str()),
                               LPCWSTR(resource_path_as_wstr.c_str()));
                efree(PHPSCITER_G(cureent_op_array));
                PHPSCITER_G(cureent_op_array) = nullptr;
            } break;
            default:
                break;
        }

        if (SCDOM_OK != SciterWindowAttachEventHandler(hw, ElementEventProcImplementeation, NULL, HANDLE_ALL))
        {
                RETURN_FALSE;
        }

        SciterShowWindow(hw);

        SciterApplicationRun(hw);

        RETURN_TRUE;
}

PHP_METHOD(phpsciter, getVersion)
{
        int len;
        char *strg;

        len = spprintf(&strg, 0, "%d", SciterVersion(FALSE));

        RETURN_STRINGL(strg, len);
}

PHP_METHOD(phpsciter, getPHPSciterVersion)
{
        RETURN_STRINGL(PHP_PHPSCITER_VERSION, strlen(PHP_PHPSCITER_VERSION));
}

PHP_METHOD(phpsciter,setResourcePath)
{
        zval *instance;

        zend_string *resource_path = NULL;
        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &resource_path) == FAILURE)
        {
                return;
        }

        instance = getThis();

        PHPSCITER_ZEND_UPDATE_PROPERTY_STRING(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_RESOURCE_PATH), ZSTR_VAL(resource_path));

        RETURN_TRUE;
}

PHP_METHOD(phpsciter,setWindowFrame)
{
        zval *instance;
        zend_long frame_top = 0;
        zend_long frame_left = 0;
        zend_long frame_right = 0;
        zend_long frame_bottom = 0;

        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &frame_top, &frame_left, &frame_right, &frame_bottom) == FAILURE)
        {
                return;
        }

        instance = getThis();

        PHPSCITER_ZEND_UPDATE_PROPERTY_LONG(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_TOP), frame_top);
        PHPSCITER_ZEND_UPDATE_PROPERTY_LONG(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_LEFT), frame_left);
        PHPSCITER_ZEND_UPDATE_PROPERTY_LONG(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_RIGHT), frame_right);
        PHPSCITER_ZEND_UPDATE_PROPERTY_LONG(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_BOTTOM), frame_bottom);

        RETURN_TRUE;
}

PHP_METHOD(phpsciter,setWindowTitle)
{
        zval *instance;

        zend_string *title = NULL;
        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &title) == FAILURE)
        {
                return;
        }

        instance = getThis();

        PHPSCITER_ZEND_UPDATE_PROPERTY_STRING(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_TITLE), ZSTR_VAL(title));

        RETURN_TRUE;
}

PHP_METHOD(phpsciter,loadFile)
{
    zval *instance;

    zend_string *file_name = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &file_name) == FAILURE)
    {
            return;
    }

    instance = getThis();
    PHPSCITER_G(loadFile) = TRUE;
    PHPSCITER_ZEND_UPDATE_PROPERTY_STRING(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_LOAD_FILE), ZSTR_VAL(file_name));

    if (PHPSCITER_G(tool)->checkPhpFile(ZSTR_VAL(file_name)) == FAILURE) {
        PHPSCITER_G(loadModal) = LOAD_HTML_FILE;
    } else {
        PHPSCITER_G(cureent_op_array) = PHPSCITER_G(tool)->zendCompileFile(ZSTR_VAL(file_name));
        PHPSCITER_G(loadModal) = LOAD_PHP_FILE;
    }

    RETURN_TRUE;
}

PHP_METHOD(phpsciter,loadHtml)
{
        zval *instance;

        zend_string *html = NULL;
        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &html) == FAILURE)
        {
                return;
        }

        instance = getThis();

        PHPSCITER_ZEND_UPDATE_PROPERTY_STRING(phpsciter_ce, instance, ZEND_STRL(PHPSCITER_PROPERTY_LOAD_HTML), ZSTR_VAL(html));
        PHPSCITER_G(loadHtml) = TRUE;
        PHPSCITER_G(loadHtml) = LOAD_HTML;

        RETURN_TRUE;
}

PHP_METHOD(phpsciter,loadPHP)
{
        zend_string *php_content = NULL;
        ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_STR(php_content)
        ZEND_PARSE_PARAMETERS_END();

        zend_op_array* op_array;
        zval php_content_zval;

        ZVAL_STR(&php_content_zval,php_content);
        PHPSCITER_G(cureent_op_array) = zend_compile_string(&php_content_zval,"Standard input code");
        PHPSCITER_G(loadModal) = LOAD_PHP;
        RETURN_TRUE;
}

void load_phpsciter_application()
{
        zend_class_entry phpsciter;

        INIT_CLASS_ENTRY(phpsciter, "phpsciter", phpsciter_methods);

#if PHP_VERSION_ID >= 70000
        phpsciter_ce = zend_register_internal_class_ex(&phpsciter, NULL);
#else
        phpsciter_ce = zend_register_internal_class_ex(&phpsciter, NULL, NULL TSRMLS_CC);
#endif

        phpsciter_ce->ce_flags = ZEND_ACC_IMPLICIT_PUBLIC;

        zend_declare_class_constant_stringl(phpsciter_ce,ZEND_STRL(PHPSCITER_INSTANCE_DEFAULT),ZEND_STRL(PHPSCITER_INSTANCE_DEFAULT) TSRMLS_CC);
        zend_declare_class_constant_stringl(phpsciter_ce,ZEND_STRL(PHPSCITER_PROPERTY_TITLE),ZEND_STRL("phpsciter") TSRMLS_CC);

        zend_declare_property_null(phpsciter_ce, ZEND_STRL(PHPSCITER_NAME), ZEND_ACC_STATIC | ZEND_ACC_PROTECTED TSRMLS_CC);

        zend_declare_property_null(phpsciter_ce, ZEND_STRL(PHPSCITER_PROPERTY_RESOURCE_PATH), ZEND_ACC_PROTECTED TSRMLS_CC);
        zend_declare_property_long(phpsciter_ce, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_TOP),0, ZEND_ACC_PROTECTED TSRMLS_CC);
        zend_declare_property_long(phpsciter_ce, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_LEFT),0, ZEND_ACC_PROTECTED TSRMLS_CC);
        zend_declare_property_long(phpsciter_ce, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_RIGHT),0, ZEND_ACC_PROTECTED TSRMLS_CC);
        zend_declare_property_long(phpsciter_ce, ZEND_STRL(PHPSCITER_PROPERTY_FRAME_BOTTOM),0, ZEND_ACC_PROTECTED TSRMLS_CC);

        //initialize static_members_table
        zend_class_init_statics(phpsciter_ce);
}