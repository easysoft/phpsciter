//
// Created by zhanglei on 2020/5/16.
//
#include "sciter-common.h"

phpsciter::Thread::Thread(phpsciter::Thread::ThreadMain mainFun, const std::string& name)
 : tMain(std::move(mainFun)),
 tName(name),
 pthread_id(0),
 tJoinStateStart(false),
 tStartState(false)
{
    setDefaultName();
}

#ifdef WINDOWS
DWORD WINAPI phpsciter::Thread::run(LPVOID obj)
{
    Thread* data = static_cast<Thread*>(obj);
    data->tMain(data->tPipe);
    return 0;
}
#elif defined(__unix__)
void* phpsciter::Thread::run(void* obj)
{
    Thread* data = static_cast<Thread*>(obj);
    data->tMain(data->tPipe);
    return nullptr;
}
#endif
bool phpsciter::Thread::start()
{
    assert(!tStartState);

    tJoinStateStart = true;

#ifdef WINDOWS
    threadHandle = CreateThread(nullptr, 0, &Thread::run, (LPVOID)this, 0, &pthread_id);
    if(!threadHandle)
    {
        zend_error(E_WARNING,strerror(errno));
        return false;
    }
    tStartState = true;
#elif defined(__unix__)
    if(pthread_create(&pthread_id, nullptr, &Thread::run, (void*)this))
    {
        zend_error(E_WARNING,strerror(errno));
        return false;
    }
#endif
    return true;
}

bool phpsciter::Thread::wait()
{

#ifdef WINDOWS
    assert(tStartState);
    int res = WaitForSingleObject(threadHandle, INFINITE);
    endState = true;
    return true;
#elif defined(__unix__)
    assert(tJoinStateStart);
    tJoinStateStart = true;
    if(pthread_join(pthread_id, nullptr) == SUCCESS)
    {
        return true;
    }else{
        zend_error(E_WARNING,strerror(errno));
        return false;
    }
#endif
}