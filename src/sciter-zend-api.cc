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

#if PHP_VERSION_ID >= 70000
int phpsciter::ZendApi::sciterWrite(const char *str, size_t str_length)
#else
int phpsciter::ZendApi::sciterWrite(const char *str, uint str_length)
#endif
{
    PHPSCITER_G(output_buffer).append(str);
    return str_length;
}

bool phpsciter::ZendApi::zendExecute()
{
    bool ret;

    if(!PHPSCITER_G(current_op_array))
    {
        zend_error(E_WARNING,"compile file error");
//        zend_bailout();
        return false;
    }

#if PHP_VERSION_ID >= 70000
    zval result;
    if (EG(exception) != NULL) {
        return false;
    }
    zend_execute(PHPSCITER_G(current_op_array), &result);
#else
    zval *result = NULL;
    storeOldExecuteInfo();
    EG(return_value_ptr_ptr) = &result;
    EG(active_op_array) 	 = PHPSCITER_G(current_op_array);
    zend_execute(PHPSCITER_G(current_op_array));
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

    if (SUCCESS != zend_stream_open(file_name, &file_handle)) {
        return  false;
    }

    if(!file_handle.opened_path)
        file_handle.opened_path = zend_string_init(file_name, strlen(file_name), 0);
//    zend_stream_init_filename(&file_handle, file_name);
#else
    file_handle.filename = file_name;
    file_handle.free_filename = 0;
    file_handle.type = ZEND_HANDLE_FILENAME;
    file_handle.opened_path = NULL;
    file_handle.handle.fp = NULL;
#endif


    zend_op_array* op_array = zend_compile_file(&file_handle, ZEND_INCLUDE);


    if(op_array && file_handle.handle.stream.handle)
    {
#if PHP_VERSION_ID < 70000
        //copy by yaf
        //http://pecl.php.net/package/yaf


#if ((PHP_MAJOR_VERSION == 5) && (PHP_MINOR_VERSION > 2)) || (PHP_MAJOR_VERSION > 5)
        if (!EG(active_symbol_table)) {
            zend_rebuild_symbol_table(TSRMLS_C);
        }
#endif
        char *opened_path = nullptr;
        int dummy = 1;
        if (!file_handle.opened_path) {
            file_handle.opened_path = opened_path = estrndup(file_name, strlen(file_name));
        }

        zend_hash_add(&EG(included_files), file_handle.opened_path, strlen(file_handle.opened_path)+1, (void *)&dummy, sizeof(int), NULL);

        if (opened_path) {
            efree(opened_path);
        }
#endif
        PHPSCITER_G(current_op_array) = op_array;
    }

    zend_destroy_file_handle(&file_handle);
    bool ret = zendExecute();
    PHPSCITER_G(request)->onClose();
#if PHP_VERSION_ID >= 70000
    if(file_handle.opened_path)
        zend_string_release(file_handle.opened_path);
#endif
    return ret;
}

phpsciter::ZendApi::~ZendApi()
{
    if(!PHPSCITER_G(output_buffer).empty())
        std::cout<<"buffer:"<<PHPSCITER_G(output_buffer)<<std::endl;
}