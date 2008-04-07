#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include <event.hpp>

//TODO:
#include <iostream>

namespace Server
{

  template< class Task >
	class CThread
	{
	public:

	  //TODO: pointer or reference?
	  CThread( Task task, bool bAutoStart = true ):m_Task( task )
												  ,m_bWorking(false)
												  ,m_WorkingEv( false, true )
		{
		  if( bAutoStart )
			Start();
		};

	  void Start()
	  {
		pthread_attr_t attr;
		pthread_attr_init( &attr );
		pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
		pthread_create( &m_thread, &attr, &CThread::ThreadFunc, this );
		pthread_attr_destroy( &attr );
	  }

	  bool IsWorking(){ return m_bWorking; }

	  void Wait()
	  {
		m_WorkingEv.Wait();
	  }

	private:

	  CThread( const CThread& );
	  CThread& operator=( const CThread& );
	  
	  Task m_Task;

	  bool m_bWorking;

	  CEvent m_WorkingEv;

	  static void* ThreadFunc( void* param )
	  {
		CThread* pThis = (CThread*)param;

		pThis->m_bWorking = true;
		pThis->m_WorkingEv.Reset();

		try
		  {
			pThis->m_Task.Execute();
		  }
		catch( std::exception& e )
		  {
			//TODO: cout >> log
			std::cout << "Thread: Uncought exception: " << e.what() << std::endl;
		  }
		catch( ... )
		  {
			std::cout << "Thread: Unknown exception" << std::endl;
		  }

		pThis->m_bWorking = false;
		pThis->m_WorkingEv.Set();

		return NULL;
	  }

	  pthread_t m_thread;

	};

}//namespace Server


#endif //THREAD_H_
