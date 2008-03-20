#ifndef SEMAPHORE_H_INCLUDED
#define SEMAPHORE_H_INCLUDED

#include <semaphore.h>
#include <sys/time.h>
#include <time.h>

class CSemaphore
{
public:

  CSemaphore( int iResourceCount )
  {
    sem_init( &m_sem, 0, iResourceCount );
  }

  ~CSemaphore()
  {
    sem_destroy( &m_sem );
  }

  void Release()
  {
    sem_post( &m_sem );
  }

  void Wait()
  {
    sem_wait( &m_sem );
  }

  //TODO: doesn`t work in FreeBSD 4.2
  /*
  bool Wait( int iTimeout )
  {
    timeval now;
    timespec timeout;

    ::gettimeofday( &now, NULL );

    timeout.tv_sec = now.tv_sec + iTimeout/1000;
    timeout.tv_nsec = now.tv_usec * 1000 + (iTimeout%1000)*1000;

    return ( 0== sem_timedwait( &m_sem, &timeout ) )?true:false;
  }
  */

  bool TryWait()
  {
    return (0 == sem_trywait( &m_sem ))?true:false;
  }

  int GetValue()
  {
    int iRes = 0;
    sem_getvalue( &m_sem, &iRes );
    return iRes;

  }
private:

  CSemaphore( const CSemaphore& );
  CSemaphore& operator=( const CSemaphore& );

  sem_t m_sem;
};


#endif // SEMAPHORE_H_INCLUDED
