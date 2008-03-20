//@(#) Berkeley DB file (code)
//@(#) Licence: LGPLv3
//@(#) Author: Andrew Wingorodov <http://andr.ru/>
//$Id: berkeley.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <berkeley.h>

using namespace std;

namespace db {

//
void
 berkeley::open (const std::string& path_, const ios::openmode mode_)
{
//	int ret= _dbp->open (_dbp, NULL, path_.c_str(),NULL, _type,_flags, 0);

//	if (0 != ret)
//		throw std::runtime_error (db_strerror (ret));

	file::_is_open = true;
}

void
 berkeley::close ()
{
//	int ret = _dbp->close (_dbp, 0);
	file::_is_open = false;
//	if (0 != ret)
//		log::s <<  "berkeley::close::DB->close error: "
//			   << db_strerror (ret) << std::endl;
}


} //::db
