//@(#) Example for use singleton
// Author: Andrew Wingorodov <http://andr.mobi/>
// Licence: LGPLv3
//$Id: example.cc 247 2008-02-08 14:48:36Z wingorodov $

#include <singleton.hpp>
#include <iostream>

using namespace std;

///\class super_duper
///\brief Using single::ton for static object
struct super_duper
{
	int duper;
	///\brief Create super-duper
	super_duper () : duper (5) {}
};

typedef single::ton<super_duper> super;

int
 main ()
{
	cout << super::it().duper << endl;
}


