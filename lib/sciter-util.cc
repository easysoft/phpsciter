//
// Created by root on 20-3-3.
//

#include "sciter-common.h"

int phpsciter::Util::isFile(const char *name) {
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

std::string phpsciter::Util::U16toString(const std::u16string &wstr)
{
    return aux::w2a(wstr).c_str();
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
    str = PHPSCITER_G(tool)->U16toString((char16_t *)quote);
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
