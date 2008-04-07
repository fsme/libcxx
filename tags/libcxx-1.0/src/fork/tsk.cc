//@(#) Contanier of process (code)
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: tsk.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <tsk.h>
#include <proc.h>

namespace sys {

//
void
 tsk::all_zombie ()
{
	for (iter pi = begin(); pi != end(); ++pi)
		pi->second->die();
}

//
void
 tsk::clear ()
{
	for (iter pi = begin(); pi != end(); ++pi)
		delete pi->second;

	map::clear();
}

} //::sys
