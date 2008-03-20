//@(#) Arguments of exec (code)
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: args.cc,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#include <args.h>

namespace sys {

//
args::constchar::constchar (const std::vector<std::string>& arg_)
{
	array = new const char* [arg_.size()+1];
	std::vector<std::string>::const_iterator i = arg_.begin();
	size_t j;

	for (j^=j; i != arg_.end (); ++i)
	{
		array[j++] = (*i).c_str() ;
	}

	array[j] = NULL;
}

} //::sys
