//@(#) Exception for report
// Licence: LGPLv3
// Author: Andrew Wingorodov <http://andr.mobi/>
//$Id: report.h 247 2008-02-08 14:48:36Z wingorodov $

#ifndef _RE_PORT_H
#define _RE_PORT_H 1

#include <exception>
#include <string>

namespace re {

///\class port
///\brief re::port For interrupt program with return code and comment
class port
	: public std::exception
{
	std::string _rep;
	int _ret;

public:
///\name Constructor and Destructor
//@{
	///\brief Create re::port
	///\param msg_ Error msg
	port ( int ret_ = 0, const std::string& rep_ = "nothing to report")
		: _ret (ret_) 
		, _rep (rep_)
	{}

	///\brief Destroy re::port
	virtual ~port () throw () {}
//@}

	///\brief Get error message
	virtual const char* what () const throw () { return _rep.c_str(); }

	///\brief Get code of return
	///\return Code of return
	int ret () const throw () { return _ret; }

	///\brief Get code of return
	///\return Code of return
	operator int () const throw () { return ret (); }

}; //.class re::port

} //::cxx

#endif //!_RE_PORT_H
