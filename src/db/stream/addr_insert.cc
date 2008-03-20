//@(#) The phonebook streambuf example (code)
// Licence: LGPLv3
// Author: Andrew Wingorodov <http://andr.ru/>
// $Id: addr_insert.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <addr_insert.h>

using namespace std;

namespace db {

//
void
 addrbuf::open (
	  const std::string path_
) {
	if ( is_open()) close();

	addrbook.open (path_, ios::out);
	this->setp ( (char*) &obuffer,(char*) (&obuffer + sizeof(obuffer)-1) );
}

//
void
 addrbuf::close()
{
	if ( !is_open()) return;

	sync ();
	this->setp (0,0);

	addrbook.close ();
}

//
int
 addrbuf::sync ()
{
	if ( !is_open()) return 0;

	if ( row.change )
	{
	cout << row.phone << endl
		 << row.fname << endl
		 << row.lname << endl
		 << row.email << endl;

		row.change = false;
	}
	this->pbump (-count());
	return 0;
}

//
int
 addrbuf::overflow (int_type c)
{
	if ( is_open ())
	{
		if ( c != EOF)
		{
			*(this->pptr()) = c;
			this->pbump(1) ;
		}
		sync ();
	}
	return c;
}

} //::db
