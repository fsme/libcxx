//@(#) Contanier of process
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: tsk.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef _SYS_TSK_H
#define _SYS_TSK_H 1

#include <map>

namespace sys {

class proc;

/** @class tsk
	@brief Container of arguments of exec functions
  */
class tsk
	: public std::map <pid_t, proc* >
{
typedef std::map <pid_t, proc* >::iterator  iter;
public:
	/// \brief Destructor: clear map of process
	~tsk ()
	{
		this->clear ();
	}

	/** @brief Insert pair into map
		@param pair_ The pair PID -> process pointer
	 */
	std::pair<iter, bool>
		insert (const std::pair<pid_t, proc* >& pair_)
	{
		return map::insert (pair_);
	}

	/// \brief Delete all process and clear the map
	void clear ();

	/// \brief Set up zombie-bit of all
	void all_zombie ();

}; //.class tsk

} //::sys

#endif //!_SYS_TSK_H
