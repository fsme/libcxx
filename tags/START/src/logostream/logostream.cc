//@(#) Output stream for log (use syslog(3))
// Licence: LGPLv3
// Author Andrew Wingorodov <http://andr.ru/>
// $Id: logostream.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <logostream.h>

namespace log {

//
log_ostrmbuf::int_type
 log_ostrmbuf::sync ()
{
	const std::streamsize count = pptr() - pbase();

	if (count > 0) {
		const std::string msg (pbase(), pptr());
		::syslog ( LOG_DEBUG,  msg.c_str ()) ;
	}

	pbump (-count);
	return 0;
}

//
log_ostrmbuf::int_type
 log_ostrmbuf::overflow (int_type c)
{
	if ( is_open ()) {
		if ( c != EOF ) {
			*(pptr()) = c;
			pbump(1) ;
		}
		sync ();
	}
	return c;
}

logostream s; ///< Global stream
} //::log
