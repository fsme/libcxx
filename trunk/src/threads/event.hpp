#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include <mutex.h>

class CEvent
{
public:

  CEvent( bool bAutoreset = true, bool bInitialState = false ):m_bAutoReset( bAutoreset )
															  ,m_bInitialState( bInitialState )
  {
    if( !bInitialState )
	  Reset();
  }


  ~CEvent()
  {};

  void Set()
  {
    m_mtx.Unlock();
  }

  void Reset()
  {
    CLock lock( m_lockmtx );
    m_mtx.TryLock();
  }

  void Wait()
  {
    CLock lock( m_lockmtx );
    m_mtx.Lock();
	if( !m_bAutoReset )
	  Set();
  }

  bool Wait( int iTimeout )
  {
    CLock lock( m_lockmtx );
    bool bRes = m_mtx.Lock( iTimeout );
    if( bRes && !m_bAutoReset )
	  Set();
    return bRes;

  }

  bool TryWait()
  {
    CLock lock( m_lockmtx );
    bool bRes = m_mtx.TryLock();
	if( bRes && !m_bAutoReset )
	  Set();
    return bRes;

  }

private:

  CMutex m_mtx;
  CMutex m_lockmtx;
  bool m_bAutoReset;
  bool m_bInitialState;
};

#endif // EVENT_H_INCLUDED
