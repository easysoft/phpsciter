//
// Created by zhanglei on 2020/5/16.
//

#ifndef PHPSCITER_SCITER_CUSTOMER_LOG_H
#define PHPSCITER_SCITER_CUSTOMER_LOG_H

enum {
    CUSTOMER_LOG_NOTICE = 0,
    CUSTOMER_LOG_WARING = 1,
    CUSTOMER_ = 2,
    CUSTOMER_LOG_SUCCESS = 3,
};
namespace phpsciter {

    class CustomerLog {

    public:
        CustomerLog(const char *dir);

        bool addLog(const char *name, const char *log, const char *file, int file_number);

        ~CustomerLog();

    private:
        int logFd;

    };
}

#endif //PHPSCITER_SCITER_CUSTOMER_LOG_H
