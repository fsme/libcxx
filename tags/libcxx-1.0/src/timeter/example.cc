//@(#) Example for use er::no
// Licence: LGPLv3
// Author Andrew Wingorodov <http://andr.mobi/>
//$Id: example.cc 592 2008-03-12 15:27:51Z wingorodov $

#include <timeter.h>

int
 main ()
{
	logs.level (level_info);
	for (int i = 10; i > 0; --i)
	{
		ti::mer bench ("bench1: ");
		::sleep(1);
		bench.here ("here is ");
		::sleep(2);
		bench.here ("here is ");
		bench.start ("bench2: ");
	}
}
