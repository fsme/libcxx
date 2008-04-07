//@(#) Fork suite
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: Fork.hpp,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef _SYS_FORK_H
#define _SYS_FORK_H 1

#include <sys/types.h>
#include <unistd.h>

#include <args.h>

namespace sys {

/** @class Fork
	@brief Suite for fork, exec* and spawn* functions
  */
template <typename ProcessType>
 class Fork
{
public:
///\name Constructor and Destructor
//@{
	///\brief Execute file
	explicit Fork (
		  const sys::args& argv_ ///\param argv_ Arguments of exec or spawn
		, const bool run = false ///\param run_ Execute child at once
	)
		: process (new ProcessType (argv_)
	) {
		if (is_parent ()) process->insert();
		if (run
		&&  is_child ()) process->exec();
	}

	///\brief Execute file
	explicit Fork (
		  const sys::args& slave_ ///\param slave_ Arguments of exec or spawn
		, const sys::args& drv_ ///\param drv_ Arguments of exec or spawn
	)
		: process ( new ProcessType (slave_, drv_)
	) {
		if (is_parent ()) process->insert();
		if (is_child ()) process->exec();
	}

	///\brief Destructor
	~Fork ()
	{
		///if ( pid() <= 0) delete process;
	}
//@}

	///\brief Execute file
	void exec () const { process->exec (); }

	///\brief True if child
	bool is_child (void) const { return process->pid() == 0; }

	///\brief True if parent
	bool is_parent (void) const { return process->pid() > 0; }

	///\brief Get Process ID
	pid_t pid (void) const { return process->pid(); }

	///\brief Get size of list of processes
	int size (void) const { return process->size(); }

	///\brief Set up termination signal
	void term (int signo_= SIGTERM ///\param signo_ Signal to interception
	) const {
		process->term (signo_);
	}

	///\brief Clean process data
	void clear () { process->clear (); }

private:
	Fork (Fork&);           ///< no copy constructor definition provided
	Fork& operator=(Fork&); ///< no assignment operator definition provided

	ProcessType* process;
	
}; //.class Fork

} //::sys

#endif //!_SYS_FORK_H
