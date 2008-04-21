//@(#) Example for use of env::iron
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: example.cc 210 2008-02-06 14:47:53Z wingorodov $

#include <string>
#include <iron.h>

#include <logostream.hpp>

using namespace std;
using namespace cxx;

int
 main (int argc, char *argv[])
{
try {
	logs << warning << "Using: example -c simple.conf" << endl;
	env::iron.getopt (argc, argv, "abc:d");
	env::iron.configure ( env::iron ("c") );
	
	logs << warning << env::iron.getenv ("HOME") << endl;

	env::iron.print (" => ");

	} catch (const std::exception& e) {
			std::cout << e.what () << std::endl;
		}
}
