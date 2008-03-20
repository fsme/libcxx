//@(#) Example for process
//@(#) Licence: LGPLv3
//@(#) Author: Andrew Wingorodov <http://andr.ru/>
//$Id: example.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <iostream>
#include <stdexcept>

#include <logostream.h>
#include <iron.h>

#include <Fork.hpp>
#include <simple.h>

using namespace std;
using namespace sys;
using namespace log;

int
 main (int argc, char* argv[])
{
try {
		env::iron.getopt (argc,argv,"p:");
		env::iron.print();

		log::s.set_level (level::info);

		sys::args arg;

		arg.path = "/bin/date";
		arg.argv.push_back ("date");
		arg.argv.push_back ("-u");
		arg.argv.push_back ("+%H:%M:%S");

		int num = 1000; 
		if ( env::iron.exists ("p"))
		{
			num = ::atoi (env::iron ("p").c_str());
		}

		for (; num > 0; --num)
		{
			Fork<simple> a (arg, true);

			if (a.size() > 100)
				throw std::runtime_error ("too many process");
		}

		while (true)
		{
			if (sys::proc::size() == 0) break;
			else
				cout << "we have "<< proc::size() << " process" << endl;

			::pause();
			std::cout << "what?" << std::endl;
		}

	} catch (err::no& e) {
			std::cout << e.what() << "#" << (int)e << std::endl;

	} catch (std::exception& e) {
			std::cout << e.what() << std::endl;
	}
}
