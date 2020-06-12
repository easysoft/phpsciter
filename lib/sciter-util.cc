//
// Created by root on 20-3-3.
//

#include "sciter-common.h"

const char* phpsciter::Util::getFileRealPath(const char *file_path) {

    if (!file_path)
    {
        return nullptr;
    }

    std::string path = file_path;
    int position = path.find("file://");
    if (EXPECTED(position == 0))
    {
        return  file_path + 7;
    } else {
        return file_path;
    }
}

int phpsciter::Util::isFile(const char *name) {
    struct stat file_stat;
    char file_name[PATH_MAX];
    char relative_dir[PATH_MAX];
    int res;


    memset(file_name, PATH_MAX, 0);
    //Absolute Path
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

int phpsciter::Util::checkPhpFile(const char* name) {
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

void phpsciter::Util::splitString(const string& subject, vector<string>& v, const string& delimiter)
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

int phpsciter::Util::strlen16(char16_t* strarg)
{
    int count = 0;
    if(!strarg)
        return -1;
    char16_t* str = (char16_t *)strarg;
    while(*str)
    {
        count++;
        str++;
    }
    return count;
}

CHAR* phpsciter::Util::ForwardToChar(LPCWSTR quote)
{
    CHAR* dest = new CHAR[1024];
    std::string str = "";
//#if defined(_WIN32)
//
//#else
//    std::u16string a(quote);
    str = PHPSCITER_G(tool)->U16toString(quote);
    dest = (CHAR *)str.data();
//#endif

    return dest;
}

LPCWSTR phpsciter::Util::ForwardToWSTR(char* quote)
{
    aux::a2w as(quote);
    return LPCWSTR(as.c_str());
}

UINT phpsciter::Util::ForwardToLength(char* quote)
{
    aux::a2w as(quote);
    return UINT(as.length());
}

time_t phpsciter::Util::getUnixTime()
{
    time_t now;
    now = time(nullptr);
    return now;
}

void phpsciter::Util::getTimeString(char* time_format)
{
#ifdef WINDOWS
    time_t now_time = getUnixTime();
    struct tm *ptr;
    ptr=localtime(&now_time);
    strcpy(time_format,asctime(ptr));
#elif defined(__unix__)
    if(!time_format)
    {
        return;
    }
    time_t now_time = getUnixTime();
    ctime_r(&now_time,time_format);
#endif
}