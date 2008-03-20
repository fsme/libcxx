//@(#) User's environment with string => string maps
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: iron.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef _ENV_IRON_H
#define _ENV_IRON_H 1

#include "ment.h"
#include <unistd.h>

namespace env {

/** @class global
	@brief User's environment
*/
class global
	: public ironment<std::string,std::string>
{
public:
//! \name Constructor and Destructor
//@{
	//! \brief By default
	inline global () {}

	//! \brief Destructor
	inline virtual ~global () {}
//@}

//! \name getopt(3)
//@{
	/** @brief Get options from a command line
		@param argc_ argc 
		@param argv_ argv
		@param optstring_ List of keys
	 */
	inline
	void getopt (int argc_, char* argv_[], const std::string optstring_)
	{
		std::string::const_iterator i;
		for ( i = optstring_.begin (); i != optstring_.end (); ++i)
		{
			if (*i == ':')
				trigger [ (int)(*(i-1)) ] = &global::getoptarg;
			else
				trigger [ (int)(*i) ] = &global::getoptkey;
		}
		int c; c ^= c;
		while  ( (c = ::getopt ( argc_, argv_, optstring_.c_str() ) ) != -1)
			if ( trigger.find (c) != trigger.end () )
				(this->*trigger[c])(c);

		argc_ -= optind;
		argv_ += optind;
		optind ^= optind;
		trigger.clear();
	}
//@}

//! \name Configuration
//@{
	/** @brief Configure environment from configuration file
		@param path_ File config
	 */
	virtual
	void
	 configure (const std::string& path_)
	{
	}
//@}

private:
	/** @brief Get option as key
		@param c_ Option without argument
	 */
	inline
	void
	 getoptkey (int c) { vars [ std::string()+(char)c ] = std::string(); }
	
	/** @brief Get option with argument
		@param c_ Option with argument
	 */
	inline
	void
	 getoptarg (int c) { vars [ std::string()+(char)c ] = ::optarg; }

	typedef void ( global::*function) (int); //!< Pointer to function
	std::map<int,function> trigger; //!< Map of functions

}; //.class global

extern global iron;

} //::env

#endif //!_ENV_IRON_H
