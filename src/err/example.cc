//@(#) Example for use err::no
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: example.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <err.h>
#include <iostream>

int
 main ()
{

	try {
		throw err::no ("hey, look: ");


	} catch (const err::no& e) {
			std::cout << e.what () << std::endl;
			std::cout << (int) e << std::endl;
		}
}
