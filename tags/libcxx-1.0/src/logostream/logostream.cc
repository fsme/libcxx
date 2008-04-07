//@(#) Output stream for log (use syslog(3))
// Licence: LGPLv3
// Author Andrew Wingorodov <http://andr.ru/>
// $Id: logostream.cc 210 2008-02-06 14:47:53Z wingorodov $

#include <logostream.hpp>

namespace cxx {

logostream<char> logs; ///< Global chars outstream

} //::cxx
