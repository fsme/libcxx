//@(#) Virtual class of process
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: proc.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef _SYS_PROC_H
#define _SYS_PROC_H 1

#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include <process.h>

#include <stdexcept>
#include <map>

#include <err.h>
#include <args.h>
#include <logostream.h>

#include <pai.h>
#include <tsk.h>

namespace sys {

/** @class proc
	@brief Virtual Suite of process (interface)
  */
class proc
	: public std::pair<pid_t, proc* >
	, virtual protected pai
{
	void constructor ();
public:
/// \name Constructor and Destructor
//@{
	/** @brief By default
		@param argv_ List of arguments
	 */
	proc (const args& argv_ = args())
		: _argv (argv_)
		, zombie (false) { constructor (); }

	/// \brief Destructor
	virtual ~proc ();
//@}

	/** @brief Execute program
	 	@param argv_ Arguments of exec* ()?
	 */
	virtual void exec (void) const;

	/** @brief Interface for SIGCHLD interception
		@param stat_ Child's status of exit 
	 */
	virtual void sig_child (int status_);
	
	/// \brief Clear all fields
	void insert ()
	{
		if ( !tasks.insert (*this).second )
			throw std::runtime_error ("PID already exists");
	}

	/** @brief Set termination signal
	 	@param signo_ Signal
	 */
	void term (int signo_) 
	{
		struct sigaction sa;
 		sa.sa_handler = (void(*)(int)) (&proc::shutdown);
 		::sigemptyset (&sa.sa_mask);
 		sa.sa_flags = 0;

		if ( ::sigaction (signo_, &sa, 0) == -1)
 	   		throw err::no ("signal fault: ");

		_sigterm = signo_;
	}

	/** @brief Get pid
		@return Process ID
	  */
	pid_t pid () const { return first; }

	/// \brief True if it is a child
	bool is_child () const { return pid() == 0; }

	/** @brief Set new pid for reincarnation 
		@param pid_  New process ID
		@return False if pid_ < 0 (::fork fault)
	  */
	bool reinc (const pid_t pid_)
	{
		first = pid_;
		return (first < 0) ? false : true;
	}

	/// \brief Set up zombie-bit
	void die () { zombie = true; }

	/// \brief Send signal to child
	/// \param signo_ Signal (SIGKILL by default)
	void kill_child (int signo_ = SIGKILL)
	{
  		if ( pid() > 0) ::kill ( pid(), signo_);
	}

	/** @brief Get size of list of processes
		@return Size of process list
	  */
	static tsk::map::size_type size ();

	/** @brief Get arguments
		@return To _argv const reference
	 */
	const args& argv () const { return _argv; }

	/// \brief Old alarm action func
	static void (*old_alrm)(int);

	/** @brief If this process has any children we kill them.
		@param signo_ Signal number
	  */
	static void shutdown (int signo_);

protected:
	args	_argv;	///< Arguments for exec/spawn functions
	bool	zombie; ///< True if process is zombie

	static tsk tasks; ///< Map of process
	static unsigned int _timeout;

	/** @brief Show status
		@param pid_ Process ID
		@param status_ Exit status
	  */
	const std::string& reason ( const pid_t pid_, const int status_ );

private:
	static int _sigterm;
	static std::map<pid_t, int> remains; ///< Map of zombies

	/** @brief Reaper nohang
		@param signo_ Signal action
	 */
	static void reaper_nohang (int signo_);

	/// \brief Beat of zombie
	static void revision (int);

}; //.class proc

} //::sys

#endif //!_SYS_PROC_H
