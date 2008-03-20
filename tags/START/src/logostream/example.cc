//@(#) Log output stream exapmle
// Licence: LGPLv3
// Author Andrew Wingorodov <http://andr.mobi/>
// $Id: example.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <logostream.h>

using namespace std;
using namespace log;

int
 main ()
{
	log::s << "You are open the logs now and write input "
			<< "from you keyboard to syslog" << endl
			<< "(type CTRL-D to close)"
			<< endl;

	///log::s.open ("test") ;
	log::s.level (level_notice);

	std::string in;
	while (std::getline (std::cin,in))
	{
		log::s << info << "discard this message" << endl
				<< critical << "example: " << in << endl;
	}

	///log::s.close ();
	log::s  << "Well, we are close connect to syslogd(8) and write messages "
	 		<< "to std::cout again. Look up result at /var/log/test.log"
			<< endl;
}
