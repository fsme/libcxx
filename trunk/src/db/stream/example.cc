//@(#) DB IO stream exapmle
// Licence LGPLv3
// Author Andrew Wingorodov <http://andr.mobi/>
// $Id: example.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <string>

#include <dbstream.h>
#include <addr_insert.h>

using namespace std;
using namespace db;

struct int2str
	: public string
{
	int2str (const u_int64_t& value_)
		: string ( (const char*) &value_, sizeof (value_)) {}
};

int
 main ()
{
try {
	addr_insert book ("/tmp/db/phones.db") ;

	book << 79031358098ULL << phone
		 << "Андрей" << first
		 << "Вингородов" << last	
		 << "mail@andr.ru" << email
		 << endl;

} catch (const std::exception& e) {
		cout << "error: " << e.what() << endl;
	}
}

