//
// Created by zhanglei on 2020/5/16.
//

/*
  +----------------------------------------------------------------------+
  | Yet Another Framework                                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Xinchen Hui  <laruence@php.net>                              |
  +----------------------------------------------------------------------+
*/
//yaf_loader_import
#include "sciter-common.h"
zend_op_array* phpsciter::ZendApi::zendCompileFile(zend_file_handle *file_handle, int type)
{
    if (!file_handle)
    {
        return nullptr;
    }

//    if(!file_handle->opened_path)
//        file_handle->opened_path = zend_string_init(file_handle->filename, strlen(file_handle->filename), 0);

//    php_printf("dir:%s\n",ZSTR_VAL(file_handle->opened_path));
//    string search_key = ZSTR_VAL(file_handle->opened_path);
//    std::map<string,zend_op_array*>::iterator iterator;
//    std::cout<<search_key<<std::endl;
//    if(PHPSCITER_G(op_array_pool).size() > 0) {
//        for(iterator=PHPSCITER_G(op_array_pool).begin(); iterator!=PHPSCITER_G(op_array_pool).end(); iterator++)
//            cout<<"key:"<<iterator->first <<endl;
//        int is_count = PHPSCITER_G(op_array_pool).count(search_key);
//        if (is_count) {
//            PHPSCITER_G(cureent_op_array) = PHPSCITER_G(op_array_pool)[search_key];
//            zend_destroy_file_handle(file_handle);
//            return PHPSCITER_G(op_array_pool)[search_key];
//        }
//    }

    zend_op_array* op_array = PHPSCITER_G(origin_zend_compile_file)(file_handle, type);
//    zend_op_array* op_array = zend_compile_file(file_handle, type);

    return op_array;
}

int phpsciter::ZendApi::sciterWrite(const char *str, size_t str_length)
{
    PHPSCITER_G(output_buffer).append(str);
    return str_length;
}

bool phpsciter::ZendApi::zendExecute()
{
    zval result;
    bool ret;

    if(!PHPSCITER_G(cureent_op_array))
    {
        zend_error(E_WARNING,"compile file error");
//        zend_bailout();
        return false;
    }

    //create an consume thread
    if(!PHPSCITER_G(output_buffer).empty())
    {
        PHPSCITER_G(output_buffer).clear();
    }

#if PHP_VERSION_ID >= 70000
    zend_execute_data *execute_data;
    if (EG(exception) != NULL) {
        return false;
    }
    zend_execute(PHPSCITER_G(cureent_op_array), &result);
#else
    zend_execute(op_array TSRMLS_DC);
#endif
    if (EG(exception))
    {
        zend_exception_error(EG(exception), E_WARNING TSRMLS_CC);
    }
#if PHP_VERSION_ID < 70000
    if (!EG(exception)) {
        if (EG(return_value_ptr_ptr) && *EG(return_value_ptr_ptr)) {
            zval_ptr_dtor(EG(return_value_ptr_ptr));
        }
    }
    reStoreOldExecuteInfo();
#endif
    return true;
}

//do php file
bool phpsciter::ZendApi::zendExecuteScript(const char* file_name, LPSCN_LOAD_DATA pc)
{
    if(!file_name)
    {
        return  false;
    }

    phpsciter::OpArrayCriticalSection op_array_guard;
    zend_file_handle file_handle;

    memset(&file_handle, sizeof(zend_file_handle), 0);

    if(pc)
    {
        PHPSCITER_G(request)->initRequest(file_name);
        PHPSCITER_G(request)->onRequest(pc);
        PHPSCITER_G(request)->onComplete();
        if(PHPSCITER_G(request)->getRequestUrl().empty())
        {
            zend_error(E_WARNING,"request url error,file:%s;line:%d",__FILE__,__LINE__);
            return  false;
        }
        file_name = PHPSCITER_G(request)->getRequestUrl().c_str();
    }

    //_fullpath(dir,filePathbuf,1024);
    file_name = PHPSCITER_G(tool)->getFileRealPath(file_name);
    if (php_check_open_basedir(file_name)) {
        return  false;
    }

#if PHP_VERSION_ID >= 70000
    if(!file_handle.opened_path)
        file_handle.opened_path = zend_string_init(file_name, strlen(file_name), 0);
#else
    if (!file_handle.opened_path) {
        file_handle.opened_path = estrdup(file_name);
    }
#endif

    if(zend_stream_open(file_name, &file_handle) != SUCCESS)
    {
        zend_error(E_WARNING,"open php file failed");
        return false;
    }

    zend_op_array* op_array = zend_compile_file(&file_handle, ZEND_INCLUDE);

#if PHP_VERSION_ID >= 70000
    if(file_handle.opened_path)
        zend_string_release(file_handle.opened_path);
#endif
    if(op_array)
    {
#if PHP_VERSION_ID < 70000

        zval *result = NULL;

        //copy by yaf
        //http://pecl.php.net/package/yaf
        storeOldExecuteInfo();
        EG(return_value_ptr_ptr) = &result;
        EG(active_op_array) 	 = op_array;

#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION > 2)) || (PHP_MAJOR_VERSION > 5)
        if (!EG(active_symbol_table)) {
            zend_rebuild_symbol_table(TSRMLS_C);
        }
#endif
#endif
//        PHPSCITER_G(op_array_pool)[search_key] = op_array;
        PHPSCITER_G(cureent_op_array) = op_array;
    }
    zend_destroy_file_handle(&file_handle);
    bool ret = zendExecute();
    PHPSCITER_G(request)->onClose();
    return ret;
}
