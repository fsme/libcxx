//@(#) Example for use env::iron
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: example.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <string>
#include <iron.h>

int
 main (\
	  int argc
	, char *argv[]
) {
	env::iron.getopt (argc, argv, "abc:d");
	env::iron.print (" => ");
	env::iron.configure ("/path/to/config");
	
	try {
	std::cout << env::iron.getenv ("PATH") << std::endl;

	} catch (const std::exception& e) {
			std::cout << e.what () << std::endl;
		}
}
