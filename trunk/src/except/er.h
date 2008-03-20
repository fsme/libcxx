//@(#) Exception with errno (3)
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: er.h 210 2008-02-06 14:47:53Z wingorodov $

#ifndef _ER_NO_H
#define _ER_NO_H 1

#include <cerrno>
#include <cstring>

#include <exception>
#include <string>

namespace er {

///\class no
///\brief Get ::errno and error msg 
class no
	: public std::exception
{
	std::string _msg;
	int _errno;

public:
///\name Constructor and Destructor
//@{
	///\brief Constructor
	///\param msg_ Error msg
	no (const std::string& msg_ = "")
		: _errno (errno) 
		, _msg (msg_)
	{
		_msg += ::strerror (_errno); }

	///\brief Destructor
	virtual ~no () throw () {}
//@}

	///\brief Get error message
	virtual
	const char* what () const throw () { return _msg.c_str(); }

	///\brief Get number of error
	///\return errno

	operator int () const throw () { return _errno; }

}; //.class er

} //::er

#endif //!_ER_NO_H
