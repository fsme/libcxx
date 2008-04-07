//@(#) Pure Abstract Interface of process
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: pai.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef _SYS_PAI_H
#define _SYS_PAI_H 1

namespace sys {

/** @class pai
	@brief Pure Abstract Interface of process 
  */
struct pai
{
	/// \brief Destructor
	virtual ~pai () = 0;

/// \name Virtual process interface
//@{
	/** @brief Execute program
	 	@param argv_ Arguments of exec* ()
	 */
	virtual void exec (void) const = 0;

	/** @brief Interface for SIGCHLD interception
		@param stat_ Child's status of exit 
	 */
	virtual void sig_child (int status_) = 0;
//@}

}; //.class pai

inline pai::~pai () {}

} //::sys

#endif //!_SYS_PAI_H
