//@(#) Example for use libcxx exceptions
// Author: Andrew Wingorodov <http://andr.mobi/>
// Licence: LGPLv3
//$Id: example.cc 247 2008-02-08 14:48:36Z wingorodov $

#include <report.h>
#include <er.h>
#include <iostream>

using namespace std;

int
 main ()
{
try {
	::fopen ("bla", "ro");
	throw er::no ("fopen error: ");

} catch (const er::no& e) {
		cout << e.what () << endl;
		cout <<  "errno=" << (int) e << endl;
	}

try {
	throw re::port (-1, "exit status=");

} catch (const re::port& e) {
		cout << e.what () << e.ret() << endl;
		return e;
	}
}
