//
// Created by zhanglei on 2020/5/16.
//

#ifndef PHPSCITER_SCITER_PIPE_H
#define PHPSCITER_SCITER_PIPE_H


namespace phpsciter {

    class Pipe :public NoCopy{
    public:
        Pipe();
        ~Pipe()
        {
            dup2(oldIn, srcIn);
            dup2(oldOut, srcOut);

#ifdef WINDOWS
            CloseHandle(hRead);
            CloseHandle(hWrite);
#elif defined(__unix__)
            close(hRead);
            close(hWrite);
#endif
        }

    bool redirectIn(int in);

    bool redirectOut(int out);

    bool finish();

#ifdef WINDOWS
    bool peekRead(DWORD dwRead)
    {
        DWORD dwAvail = 0;
        return (PeekNamedPipe(hRead, NULL, NULL, &dwRead, &dwAvail, NULL) || dwAvail <= 0);
    }

    bool read(char* buf, size_t buf_size, DWORD* dwRead)
    {
        return ReadFile(hRead, buf, buf_size, dwRead, nullptr);
    }

#elif defined(__unix__)
    bool setNoBlockIn()
    {
        inFlags = fcntl(hRead, F_GETFL, 0);
        return fcntl(hRead, F_SETFL, inFlags | O_NONBLOCK);
    }

    size_t read(char* buf,size_t buf_size,size_t* read_size)
    {
        *read_size = ::read(hRead, buf, BUFSIZ);
        return *read_size;
    }
#endif

    private:
#ifdef WINDOWS
        HANDLE hRead;
        HANDLE hWrite;
        int read_fd;
        int write_fd;
#elif defined(__unix__)
        int hRead;
        int hWrite;
#endif
        int oldIn;
        int oldOut;
        int srcIn;
        int srcOut;
        int inFlags;
    };
}
#endif //PHPSCITER_SCITER_PIPE_H
