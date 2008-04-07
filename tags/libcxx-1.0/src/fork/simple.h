//@(#) Simple process for example
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: simple.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef _SYS_SIMPLE_H
#define _SYS_SIMPLE_H 1

#include <proc.h>
#include <logostream.h>

namespace sys {

/** @class simple
	@brief simple implementation of process
  */
struct simple
	: public proc
{
explicit
	simple (const args& argv_) : proc (argv_) {}
	virtual ~simple () {}

void
 sig_child (int status_)
{
	log::s << "process " << pid() << " return " << status_ << std::endl;

	proc::die ();
	tasks.erase ( pid());
}

void exec (void) const
{
	if ( ::execve (\
		  proc::argv().path.c_str()
		, args::constchar (proc::argv().argv) 
		, args::constchar (proc::argv().envp) ) == -1
	)
	throw err::no ("execve: ");
}

}; //.class simple

} //::sys

#endif //!_SYS_SIMPLE_H
