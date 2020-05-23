//
// Created by zhanglei on 2020/5/16.
//

#ifndef PHPSCITER_SCITER_PIPE_H
#define PHPSCITER_SCITER_PIPE_H


namespace phpsciter {

    class Pipe {
    public:
        Pipe();

        Pipe(const Pipe& src_pipe);

        ~Pipe()
        {
            if(oldIn > 0 && srcIn > 0)
            {
                if(dup2(oldIn, srcIn) != -1)
                {
                    close(oldIn);
                }
            }

            if(oldOut > 0 && srcOut > 0)
            {
                if(dup2(oldOut, srcOut) != -1)
                {
                    close(oldOut);
                }
            }

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

        size_t finish();

    #ifdef WINDOWS
        bool peekRead(DWORD dwRead)
        {
            DWORD dwAvail = 0;
            return (PeekNamedPipe(hRead, NULL, NULL, &dwRead, &dwAvail, NULL) || dwAvail <= 0);
        }

        DWORD read(char* buf, size_t buf_size, DWORD* dwRead)
        {
            if(ReadFile(hRead, buf, buf_size, dwRead, nullptr))
            {
                return *dwRead;
            }else{
                return 0;
            }
        }

        HANDLE getReadHandle() const
        {
            return hRead;
        }

        HANDLE getWriteHandle() const
        {
            return hWrite;
        }

        bool setReadHandle(HANDLE read)
        {
            return hRead = read;
        }

        bool setWriteHandle(HANDLE write)
        {
            return hWrite = write;
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

        int getReadHandle() const
        {
            return hRead;
        }

        int getWriteHandle() const
        {
            return hWrite;
        }

        bool setReadHandle(int read)
        {
            return hRead = read;
        }

        bool setWriteHandle(int write)
        {
            return hWrite = write;
        }
    #endif

        bool setOldIn(int in)
        {
            return oldIn = in;
        }

        int getOldIn() const
        {
            return oldIn;
        }

        bool setOldOut(int out)
        {
            return oldOut = out;
        }

        int getOldOut() const
        {
            return oldOut;
        }

        bool setSrcIn(int in)
        {
            return srcIn = in;
        }

        int getSrcIn() const
        {
            return srcIn;
        }

        bool setSrcOut(int out)
        {
            return srcOut = out;
        }

        int getSrcOut() const
        {
            return srcOut;
        }

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
        int oldIn = -1;
        int oldOut = -1;
        int srcIn = -1;
        int srcOut = -1;
        int inFlags = -1;
    };
}
#endif //PHPSCITER_SCITER_PIPE_H
