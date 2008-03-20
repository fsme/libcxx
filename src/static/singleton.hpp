//@(#) Singleton template
// Author: Dmitry Parshin
// Licence: LGPLv3
//$Id: singleton.hpp 248 2008-02-08 14:58:14Z wingorodov $

#ifndef _SINGLE_TON_H
#define _SINGLE_TON_H 1

namespace single {

///\class ton
///\brief Singleton template

template< class T >
struct ton
{
	static T& it ()
	{
		static T instance;
		return instance;
	}
};

} //::single

#endif ///_SINGLE_TON_H
