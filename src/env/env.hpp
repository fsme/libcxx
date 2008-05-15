//@(#) Program environment
// Licence: LGPLv3
// Author: Andrew Wingorodov <http://andr.mobi/>
//$Id$

#ifndef _ENV_RON_HPP
#define _ENV_RON_HPP 1

#include <unistd.h>
#include <ctype.h>

#include <fstream>

#include <ment.hpp>

using namespace std;
using namespace cxx;

namespace env {

///\brief It is not a space
///\return True if it is not a space

inline
bool isnt_space (int sp_) ///\param sp_ Char for test
{
	return  (::isspace (sp_)) ? false : true;
}

///\class global
///\brief User's environment

class global
	: public ironment<std::string, std::string>
{
public:
///\name Constructor and Destructor
//@{
	///\brief By default
	global () {}

	///\brief Destructor
	virtual ~global () {}
//@}

///\brief Get options from a command line
void getopt (
	  int& argc_///\param argc_ Arguments counter 
	, char**& argv_///\param argv_ Argvuments vector of string
	, const std::string optstring_ ///\param optstring_ List of keys
) {
	opterr = 0;
	std::string::const_iterator i;
	for ( i = optstring_.begin (); i != optstring_.end (); ++i)
	{
		if (*i == ':') trigger [ (int)(*(i-1)) ] = &global::getoptarg;
		else
			trigger [ (int)(*i) ] = &global::getoptkey;
	}
	int c;
	while  ( (c = ::getopt ( argc_, argv_, optstring_.c_str() ) ) != -1)
		if ( trigger.find (c) != trigger.end () )
			(this->*trigger[c])(c);

	argc_ -= optind;
	argv_ += optind;
	trigger.clear();
}

///\name Configuration
//@{
	///\brief Configure environment from configuration file
	virtual
	void configure (
		 const std::string& path_///\param path_ File config
	) {
		std::ifstream cfg (path_.c_str());
		if ( !cfg.is_open ())
			throw er::no (path_ + " configure error: ");

		std::string _val;

		while ( std::getline( cfg, _val))
		{
			std::string::size_type pos;
			if ( (pos=_val.find(':')) == string::npos) continue;

			std::string _key ( _val.begin (), _val.begin()+pos );
			_val.erase ( _val.begin(), _val.begin()+pos+1 );

			//-- Remove spaces from key
			_key.erase (
				  std::remove_if ( _key.begin(), _key.end(), ::isspace)
				, _key.end()
			);

			//-- Remove spaces after colon
			_val.erase (
				  _val.begin ()
				, std::find_if ( _val.begin(), _val.end(), isnt_space)
			);

			operator[] (_key) = _val;
		}
	}
//@}

private:

	global ( const global& );
	global::operator= ( const global& );

	///\brief Get option as key
	void
	 getoptkey (
		 int c_ ///\param c_ Option without argument
	) {
		vars [ std::string()+(char)c_ ] = std::string(); }
	
	///\brief Get option with argument
	void
	 getoptarg (
		int c_ ///\param c_ Option with argument
	) {
		vars [ std::string()+(char)c_ ] = ::optarg; }

	typedef void ( global::*function) (int); ///< Pointer to function
	std::map<int,function> trigger; ///< Map of functions

}; //.class global

extern global iron;
} //::env

#endif //!_ENV_RON_HPP
