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
zend_op_array* phpsciter::ZendApi::zendCompileFile(const char* file_name)
{
    zend_file_handle file_handle;

    memset(&file_handle, sizeof(zend_file_handle), 0);
    if(zend_stream_open(file_name, &file_handle) != SUCCESS)
    {
        zend_error(E_WARNING,"open php file failed");
        return nullptr;
    }

#if PHP_VERSION_ID >= 70000
    if(!file_handle.opened_path)
        file_handle.opened_path = zend_string_init(file_name, strlen(file_name), 0);
#else
    if (!file_handle.opened_path) {
        file_handle.opened_path = estrdup(file_name);
    }
#endif

    PHPSCITER_G(cureent_op_array) = zend_compile_file(&file_handle, ZEND_INCLUDE_ONCE);

    zend_destroy_file_handle(&file_handle);
#if PHP_VERSION_ID >= 70000
    if(file_handle.opened_path)
        zend_string_release(file_handle.opened_path);
#endif

#if PHP_VERSION_ID < 70000
    if(op_array)
    {
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
    }
#endif
    return PHPSCITER_G(cureent_op_array);
}

void consumeThreadMain(std::shared_ptr<phpsciter::Pipe> pipe)
{
    ssize_t size;
#ifdef WINDOWS
    DWORD read_bytes;
#elif defined(__unix__)
    size_t read_bytes;
#endif
    char buf[BUFSIZ];
    std::weak_ptr<phpsciter::Pipe> w_pipe(pipe);
    if(w_pipe.lock())
    {
        errno = 0;

        while ((size = pipe->read( buf, BUFSIZ, &read_bytes)) > 0) {
            if (errno == EINTR)
                continue;
            else if (errno == EAGAIN) {
                buf[size] = '\0';
                break;
            } else {
                buf[size] = '\0';
                PHPSCITER_G(zend)->getBuffer().append(buf);
                continue;
            }
        }
    }
}



bool phpsciter::ZendApi::zendExecute()
{
    zval result;
    bool ret;
    phpsciter::OpArrayCriticalSection op_array_guard;
    std::shared_ptr<phpsciter::Pipe> consume_pipe = std::make_shared<phpsciter::Pipe>();
    phpsciter::Thread thread_handle(consumeThreadMain);

    buffer.clear();

    thread_handle.setPipe(consume_pipe);
    ret = consume_pipe->redirectOut(STDOUT_FILENO);

    if(!ret)
    {
        return false;
    }

#ifdef __unix__
    consume_pipe->setNoBlockIn();
#endif

    if(!PHPSCITER_G(cureent_op_array))
    {
        zend_error(E_WARNING,"compile file error");
        zend_bailout();
        return false;
    }

    if(!thread_handle.start())
    {
        return  false;
    }
    //create an consume thread
#if PHP_VERSION_ID >= 70000
    zend_execute(PHPSCITER_G(cureent_op_array),&result);
#else
    zend_execute(op_array TSRMLS_DC);
#endif
    if (EG(exception))
    {
        zend_exception_error(EG(exception), E_ERROR TSRMLS_CC);
    }
    //wait thread destory
    if(!thread_handle.wait())
    {
        return false;
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

