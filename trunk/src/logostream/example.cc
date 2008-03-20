//@(#) Log output stream exapmle
// Licence: LGPLv3
// Author Andrew Wingorodov <http://andr.mobi/>
// $Id: example.cc 590 2008-03-12 14:59:33Z wingorodov $

#include <logostream.hpp>

using namespace std;
using namespace cxx;

int
 main ()
{
	logs << "You are open the logs now and write input " << endl
		 << "from you keyboard to syslog (type CTRL-D to close)"
		 << endl;

	logs.open ("test") ;
	logs.level (level_notice);

	std::string in;
	while (std::getline (std::cin,in))
	{
		logs << error << "error seen " << in << endl;
		logs << warning << "warning seen " << in << endl;
		logs << notice << "notice seen " << in << endl;
		logs << info << "info hide " << in << endl;
		logs << debug << "debug hide " << in << endl;

		if (logs.is (level_warning))
			logs << warning << "is (level_warning) seen " << in << endl;

		if (logs << notice)
			logs << "if (notice) seen " << in << endl;

		if (logs << info)
			logs << "if (info) hide " << in << endl;
	}

	logs.close ();
	logs << "Well, we are close connect to syslogd(8) and write messages "
	 	 << "to std::cout again. Look up result at /var/log/test.log"
		 << endl;
}
