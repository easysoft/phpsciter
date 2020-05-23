//
// Created by zhanglei on 2020/5/16.
//

//
// Created by zhanglei on 19-8-14.
//
#include "sciter-common.h"

phpsciter::CustomerLog::CustomerLog(const char* dir)
{
    logFd = open(dir,O_RDWR|O_CREAT|O_APPEND);
    if(logFd == -1)
    {
        cout<<"[CServiceLog::CServiceLog]:open log "<<dir<<" failed;errno"<<errno<<";errormsg:"<<strerror(errno)<<";file:"<<__FILE__<<";line:"<<__LINE__;
        exit(-1);
    }
}

/**
 * @param name
 * @param log
 * @param file
 * @param file_number
 * @return
 */
bool phpsciter::CustomerLog::addLog(const char* name,const char* log,const char* file,int file_number)
{
    ssize_t res;
    char buf[BUFSIZ];
    memset(buf, sizeof(buf), 0);
    char time_buf[26];
    PHPSCITER_G(tool)->getTimeString(time_buf);
    int time_len = strlen(time_buf);
    if(time_len <= 0)
    {
        return false;
    }
    time_buf[strlen(time_buf) - 1] = '0';
    snprintf(buf,sizeof(buf),"<%s> [%s]:%s;errno:%d;errormsg:%s;file:%s;line:%d\n",time_buf,name,log,errno,strerror(errno),file,file_number);
    cout<<buf;
    res = write(logFd,buf, strlen(buf));
    if(res == -1)
    {
        return  false;
    }
    return true;
}

phpsciter::CustomerLog::~CustomerLog()
{
    if(logFd > 0)
    {
        close(logFd);
    }
}