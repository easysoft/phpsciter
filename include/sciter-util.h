//
// Created by root on 20-3-7.
//

#ifndef PHPSCITER_PHPSCITER_UTIL_H
#define PHPSCITER_PHPSCITER_UTIL_H


namespace phpsciter {

    class Util :public NoCopy{

    public:

        int isFile(const char *name);

        int checkPhpFile(const char *name);

        const char* getFileRealPath(const char *file_path);

        int getErrno() {
            return this->error_number;
        }

        const char *getError() {
            return this->error.c_str();
        }

        std::string U16toString(LPCWSTR wstr)
        {
            return aux::w2a(wstr).c_str();
        }

        void splitString(const std::string &subject, std::vector <std::string> &v, const std::string &delimiter);

        static void *consumeThread(void *args);

        int strlen16(char16_t* strarg);

        CHAR* ForwardToChar(LPCWSTR quote);

        LPCWSTR ForwardToWSTR(char* quote);

        UINT ForwardToLength(char* quote);

        time_t getUnixTime();

        void getTimeString(char* time_format);

    private:

        int error_number = 0;

        std::string error;

        zend_bool setUnixError(int err) {
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
}
#endif //PHPSCITER_PHPSCITER_UTIL_H
