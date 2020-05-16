//
// Created by zhanglei on 2020/5/16.
//
#include "sciter-common.h"

phpsciter::Pipe::Pipe()
{
    int ret;
#ifdef __unix__
    int pipe_fd[2];
    ret = pipe(pipe_fd);
    if(ret == SUCCESS)
    {
        hRead = pipe_fd[0];
        hWrite = pipe_fd[1];
    }else{
        zend_error(E_WARNING,strerror(errno));
    }
#elif WINDOWS
    res = CreatePipe(&hRead, &hWrite, nullptr, 0);
    if(res <= 0)
    {
        zend_error(E_WARNING,strerror(errno));
    }
#endif
}

bool phpsciter::Pipe::redirectIn(int in)
{
    int res;

    srcIn = in;

    oldIn = dup(in);
    if(oldIn < 0)
    {
        return  false;
    }

#ifdef __unix__
    res = dup2(hWrite, in);
    if(res < 0)
    {
        zend_error(E_WARNING,strerror(errno));
        return false;
    }

#elif WINDOWS
    read_fd =_open_osfhandle((INT_PTR)hRead, _O_TEXT);
    if(read_fd < 0)
    {
        zend_error(E_WARNING,strerror(errno));
        return false;
    }

    res = dup2(read_fd, in);
    if(read_fd < 0)
    {
        zend_error(E_WARNING,strerror(errno));
        return false;
    }
#endif

    return true;
}

bool phpsciter::Pipe::redirectOut(int out)
{
    int res;

    srcOut = out;

    oldOut = dup(out);
    if(oldOut < 0)
    {
        return  false;
    }

#ifdef __unix__
    res = dup2(hWrite, out);
    if(res < 0)
    {
        zend_error(E_WARNING,strerror(errno));
        return false;
    }
#elif WINDOWS
    write_fd =_open_osfhandle((INT_PTR)hWrite, _O_TEXT);
    if(write_fd < 0)
    {
        zend_error(E_WARNING,strerror(errno));
        return false;
    }

    res = dup2(write_fd, out);
    if(write_fd < 0)
    {
        zend_error(E_WARNING,strerror(errno));\
        return false;
    }
#endif

    return true;
}
