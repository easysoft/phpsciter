//
// Created by root on 20-3-7.
//

#ifndef PHPSCITER_PHPSCITER_UTIL_H
#define PHPSCITER_PHPSCITER_UTIL_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <zend_types.h>
#include <zend_compile.h>

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

    std::string U16toString(const std::u16string &wstr);

private:

    int error_number = 0;

    string error;

    zend_bool setUnixError(int err)
    {
        this->error_number = err;
        this->error = strerror(err);
    }

    void setErrno(int err) {
        this->error_number = err;
    }

    void setError(const char *buf) {
        this->error = buf;
    }
};

#endif //PHPSCITER_PHPSCITER_UTIL_H
