//
// Created by zhanglei on 2020/5/16.
//
#include "sciter-common.h"

phpsciter::Pipe::Pipe()
{
#ifdef __unix__
    int ret;
    int pipe_fd[2];
    ret = pipe(pipe_fd);
    if(ret == SUCCESS)
    {
        hRead = pipe_fd[0];
        hWrite = pipe_fd[1];
    }else{
        zend_error(E_WARNING,strerror(errno));
    }
#elif defined(WINDOWS)
    bool res = CreatePipe(&hRead, &hWrite, nullptr, 0);
    if(!res)
    {
        zend_error(E_WARNING,strerror(errno));
    }
#endif
}

phpsciter::Pipe::Pipe(const phpsciter::Pipe& src_pipe)
{
    setOldIn(src_pipe.getOldIn());
    setOldOut(src_pipe.getOldOut());
    setSrcIn(src_pipe.getSrcIn());
    setSrcOut(src_pipe.getSrcOut());
    setReadHandle(src_pipe.getReadHandle());
    setWriteHandle(src_pipe.getWriteHandle());
}

bool phpsciter::Pipe::redirectIn(int in)
{
    int res;

    setSrcIn(srcIn);

    int copy_in = dup(in);
    if(copy_in < 0)
    {
        return  false;
    }

    setOldIn(copy_in);

#ifdef __unix__
    res = dup2(hWrite, in);
    if(res < 0)
    {
        zend_error(E_WARNING,strerror(errno));
        return false;
    }

#elif defined(WINDOWS)
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

size_t phpsciter::Pipe::finish()
{
    size_t n = FINISH_EOF_LEN;
    const void *vptr = FINISH_EOF;
    size_t          nleft = FINISH_EOF_LEN;
    const char*    ptr = (char*)vptr;

#ifdef WINDOWS
    DWORD nwrite = 0;

    while (nleft > 0)
    {
        if ((WriteFile(hWrite, vptr, nleft ,&nwrite, nullptr)) <= 0)
        {
            if (nwrite < 0 && EINTR == errno)
                nwrite = 0;
            else
                return -1;
        }
        nleft -= nwrite;
        ptr += nwrite;
    }
    return nwrite;
#elif defined(__unix__)

    ssize_t        nwrite = 0; //write函数本次向fd写的字节数

    while (nleft > 0)
    {
        if ((nwrite = write(hWrite, ptr, nleft)) <= 0)
        {
            if (nwrite < 0 && EINTR == errno)
                nwrite = 0;
            else
                return -1;
        }
        nleft -= nwrite;
        ptr += nwrite;
    }
    return nwrite;
#endif
}


bool phpsciter::Pipe::redirectOut(int out)
{
    int res;

    setSrcOut(out);

    int copy_out = dup(out);
    if(copy_out < 0)
    {
        return  false;
    }

    setOldOut(copy_out);

#ifdef __unix__
    res = dup2(hWrite, out);
    if(res < 0)
    {
        zend_error(E_WARNING,strerror(errno));
        return false;
    }
#elif defined(WINDOWS)
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
