//
// Created by zhanglei on 2020/5/16.
//

#ifndef PHPSCITER_SCITER_THREAD_H
#define PHPSCITER_SCITER_THREAD_H

#include "sciter-common.h"

namespace phpsciter{

    class Thread :public NoCopy{

    public:

        typedef std::function<void (std::shared_ptr<Pipe> pipe)> ThreadMain;

        explicit Thread(ThreadMain, const std::string& name = std::string());

        bool setPipe(std::shared_ptr<Pipe> pipe)
        {
            tPipe = pipe;
            return  true;
        }

#ifdef WINDOWS
        DWORD WINAPI run(LPVOID arg);

        ~Thread()
        {
            assassert(endState);
        }
#elif __unix__
        static void* run(void* arg);

        ~Thread()
        {
            if(tJoinStateStart && !tJoinStateStart)
            {
                pthread_detach(pthread_id);
            }
        }
#endif

        bool start();

        bool wait();

    private:

        bool setDefaultName()
        {
            if(tName.empty()) {
#ifdef WINDOWS
                tName = gettid();
#elif __unix__
                tName = pthread_self();
#endif
            }
            return  true;
        }

        ThreadMain tMain;

        std::string tName;

        int tStartState;

#ifdef WINDOWS
        int endState = false;
        HANDLE threadHandle = nullptr;
        DWORD  pthread_id;
#elif __unix__
        int tJoinStateStart;
        pthread_t pthread_id;
#endif
        std::shared_ptr<Pipe> tPipe = nullptr;
    };
}

#endif //PHPSCITER_SCITER_THREAD_H
