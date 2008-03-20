#ifndef __MUTEX
#define __MUTEX

#include <pthread.h>


class CMutex
{
public:
    CMutex();
    ~CMutex();

    void Lock();
    bool Lock( int iTime );
    bool TryLock();

    void Unlock();


private:

    pthread_mutex_t m_mtx;
    pthread_cond_t m_cond;
    pthread_mutex_t m_condmtx;
};

class CLock
{
public:
    CLock( CMutex& mtx ):m_mtx(mtx)
    {
        m_mtx.Lock();
    }

    ~CLock()
    {
        m_mtx.Unlock();
    }
private:

    CMutex& m_mtx;
};
#endif

