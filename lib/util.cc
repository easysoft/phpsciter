//
// Created by root on 20-3-3.
//

#include "util.h"
#include "php_phpsciter.h"
#include <fcntl.h>
#include <php.h>

int Util::isFile(const char *name) {
    struct stat file_stat;
    char file_name[PATH_MAX];
    char relative_dir[PATH_MAX];
    int res;


    bzero(file_name,PATH_MAX);
    //这是一个绝对路径
    strcpy(file_name,name);
    res = stat(file_name,&file_stat);
    if (res == SUCCESS) {
        if (S_ISREG(file_stat.st_mode)) {
            return SUCCESS;
        } else {
            this->setErrno(CUSTOMER_ERROR);
            this->setError((error+"Filename must be a normal file;file path"+file_name).c_str());
            return FAILURE;
        }
    } else {
        string error(strerror(errno));
        this->setErrno(CUSTOMER_ERROR);
        this->setError((error + ";"+file_name).c_str());
        return FAILURE;
    }
}

int Util::checkPhpFile(const char* name) {
    const char *ext;
    size_t ext_len;
    ext_len = strlen(name);
    if(ext_len < 4) {
        return FAILURE;
    }else {
        ext = name + (ext_len - 4);
        if(strcmp(ext,".php") == SUCCESS)
        {
            return SUCCESS;
        }else{
            return FAILURE;
        }
    }
}

string Util::zendExecute(zend_op_array *op_array)
{
    zval result;
    int pipe_fd[2];
    int res;
    char buf[BUFSIZ];
    int flags;
    ssize_t size;
    string content;
    int stdout_fd;

    stdout_fd = dup(STDOUT_FILENO);

    content.clear();
    res = pipe(pipe_fd);
    if(res != SUCCESS) {
        this->setUnixError(errno);
        zend_error(E_ERROR,this->getError());
    }

    res = dup2(pipe_fd[1],STDOUT_FILENO);
    if(res < SUCCESS)
    {
        this->setUnixError(errno);
        zend_error(E_ERROR,this->getError());
    }

    if(!op_array)
    {
        this->setErrno(CUSTOMER_ERROR);
        this->setError("failed to open stream: No such file or directory");
        zend_bailout();
    }
#if PHP_VERSION_ID >= 70000
    zend_execute(op_array,&result);
#else
    zend_execute(op_array TSRMLS_DC);
#endif
    flags = fcntl(pipe_fd[0],F_GETFL,0);
    fcntl(pipe_fd[0],F_SETFL,flags|O_NONBLOCK);
    errno = 0;
    while((size = read(pipe_fd[0],buf,BUFSIZ)) > 0)
    {
        if(errno == EINTR)
            continue;
        else if(errno == EAGAIN) {
            buf[size] = '\0';
            break;
        }else{
            buf[size] = '\0';
            content.append(buf);
            continue;
        }
    }

    dup2(stdout_fd,STDOUT_FILENO);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    destroy_op_array(op_array TSRMLS_CC);
#if PHP_VERSION_ID < 70000
    if (!EG(exception)) {
        if (EG(return_value_ptr_ptr) && *EG(return_value_ptr_ptr)) {
            zval_ptr_dtor(EG(return_value_ptr_ptr));
        }
    }
    reStoreOldExecuteInfo();
#endif
    return content;
}

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
zend_op_array *Util::zendCompileFile(const char* file_name)
{
    zend_file_handle file_handle;

    bzero(&file_handle, sizeof(zend_file_handle));
    if(zend_stream_open(file_name, &file_handle) != SUCCESS)
    {
        this->setErrno(CUSTOMER_ERROR);
        this->setError("open php file failed");
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

    zend_op_array *op_array = zend_compile_file(&file_handle, ZEND_INCLUDE_ONCE);

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
    return op_array;
}

std::string Util::U16toString(const std::u16string &wstr)
{
    return aux::w2a(wstr).c_str();
}

void Util::splitString(const string& subject, vector<string>& v, const string& delimiter)
{
    string::size_type pos1, pos2;
    pos2 = subject.find(delimiter);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(subject.substr(pos1, pos2-pos1));

        pos1 = pos2 + delimiter.size();
        pos2 = subject.find(delimiter, pos1);
    }
    if(pos1 != subject.length())
        v.push_back(subject.substr(pos1));
}