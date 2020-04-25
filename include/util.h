//
// Created by root on 20-3-7.
//

#ifndef PHPSCITER_PHPSCITER_UTIL_H
#define PHPSCITER_PHPSCITER_UTIL_H

#ifndef PHP_VERSION_ID
#include "php_version.h"
#endif
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <zend_types.h>
#include <zend_compile.h>
#include <vector>

using namespace std;

enum {
    CUSTOMER_ERROR = 1001,
};

class Util{

public:

    int isFile(const char* name);

    int checkPhpFile(const char* name);

    zend_op_array * zendCompileFile(const char* file_name);

    string zendExecute(zend_op_array *op_array);

    int getErrno() {
        return this->error_number;
    }

    const char *getError() {
        return this->error.c_str();
    }

    string U16toString(const std::u16string &wstr);

    void splitString(const string& subject, vector<string>& v, const string& delimiter);

private:

    int error_number = 0;

    string error;

    zval ** __old_return_value_pp;

    zend_op ** __old_opline_ptr;

    zend_op_array * __old_op_array;

#if PHP_VERSION_ID < 70000
    void storeOldExecuteInfo()
    {
        __old_return_value_pp   = EG(return_value_ptr_ptr);
		__old_opline_ptr  	= EG(opline_ptr);
		__old_op_array  = EG(active_op_array);
    }

    void reStoreOldExecuteInfo()
    {
        EG(return_value_ptr_ptr) = __old_return_value_pp;
		EG(opline_ptr)			 = __old_opline_ptr;
		EG(active_op_array)		 = __old_op_array;
    }
#endif

    zend_bool setUnixError(int err)
    {
        this->error_number = err;
        this->error = strerror(err);
        return SUCCESS;
    }

    void setErrno(int err) {
        this->error_number = err;
    }

    void setError(const char *buf) {
        this->error = buf;
    }
};

#endif //PHPSCITER_PHPSCITER_UTIL_H
