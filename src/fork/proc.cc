//@(#) Fork process code
//@(#) Licence: LGPLv3
//@(#) Author: Andrew Wingorodov <http://andr.ru/>
//$Id: proc.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <proc.h>

using namespace log;

namespace sys {

tsk proc::tasks;
tsk::map::size_type proc::size () { return proc::tasks.size(); }

typedef std::map<pid_t, int> zombies;
zombies proc::remains;

int proc::_sigterm = 0;
unsigned int proc::_timeout = 1;
void (*proc::old_alrm)(int);

//
void
 proc::constructor ()
{
	sigset_t mask;
 	if (_sigterm != 0) ///< maskoff interrupts
	{
		::sigaddset (&mask, _sigterm);
		::sigprocmask (SIG_BLOCK, &mask, NULL);
	}

	first = ::fork ();
	if ( pid() < 0)
		throw err::no ("fork error: ");

	if ( pid() == 0) {
		tasks.all_zombie ();
		tasks.clear ();
	}
	else 
		if ( size() == 0)
		{
			struct sigaction sa;
			sa.sa_handler = (void(*)(int)) (&proc::reaper_nohang);
			::sigemptyset (&sa.sa_mask);
			sa.sa_flags = 0;
			::sigaction (SIGCHLD, &sa, 0);

			old_alrm = ::signal (SIGALRM, &proc::revision );
			::alarm (_timeout);
		}

	second = this;

 	if (_sigterm != 0) ///< mask on interrupt
		::sigprocmask (SIG_UNBLOCK, &mask, NULL);

	{
		struct sigaction sa;
		sa.sa_handler = (void(*)(int)) (&proc::shutdown);
		::sigemptyset (&sa.sa_mask);
		sa.sa_flags = 0;

		if ( ::sigaction (SIGTERM, &sa, 0) == -1)
			throw err::no ("sigaction fault ");
	}
}

/// virtual destructor
 proc::~proc ()
{
	if (pid() > 0)
	{
		tsk::iterator i = tasks.begin();
		for (; i != tasks.end(); ++i)
		{
			if ( i->first == pid() && !zombie)
			{
				kill_child (SIGINT);
				break;
			}
		}
	}
}

//
const std::string&
 proc::reason (const pid_t pid_, const int status_ )
{
	static std::string stat;
	stat.resize(0);

	if ( pid() <= 0) return stat;

/***
	if (WIFSTOPPED (status_))
	{
	   	stat += "process gets "
				////<< SYS_SIGLIST [WSTOPSIG (status())]
				;
		} else
			if (WIFEXITED (status_) )
			{
 			   sstr  << "process " << pid() << " exited with status "
						<< WEXITSTATUS (status_);
			} else
				if (WIFSIGNALED ( status_) )
				{
 				   sstr << "process " << pid() << " got "
						 /// << SYS_SIGLIST [WTERMSIG (status_)]
						 ;
				}
	}
***/
	return stat;
}

// virtual
void
 proc::sig_child (int status_)
{
	log::s << "virtual proc " << pid() << " return" << status_ << std::endl;
}

// virtual
void
 proc::exec (void) const
{
	log::s << "virtual exec an undefined" << std::endl;
}

//
void
 proc::reaper_nohang (int signo_)
{
	if (signo_ != SIGCHLD) return;

	int status;
	pid_t pid;

	while ( (pid = ::waitpid (-1, &status, WNOHANG)) > 0)
	{
		remains [pid] = status;
	}
}
		
//
void
 proc::revision (int signo_)
{
	tsk::iterator i = tasks.begin();
	for (; i != tasks.end(); ++i)
	{
 		zombies::iterator z;
		if ( (z = remains.find (i->first)) != remains.end() )
		{
			i->second->sig_child (z->second);
			remains.erase (z);
			i = tasks.begin();
		}
	}

	if (signo_ == SIGALRM) {
cout << "alarm (" << _timeout << ")" << endl;
		::alarm (_timeout);
	}
}

//
void
  proc::shutdown (int signo_)
{
	log::s << "shutdown by signal " << signo_ << std::endl;

	tsk::iterator i = tasks.begin();
	for (; i != tasks.end(); ++i)
	{
		::kill (i->first, SIGTERM);
	}

	::exit (signo_); 
}

} //::sys
