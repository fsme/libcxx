//@(#) Shell for errno
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: err.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef _ERR_NO_H
#define _ERR_NO_H 1

#include <cerrno>
#include <cstring>

#include <exception>
#include <string>

namespace err {

/** @class no
	@brief Get ::errno and message of error 
*/
class no
	: public std::exception
{
public:
//! \name Constructor and Destructor
//@{
	/** @brief Constructor
		@param message_ Application's message about error
	 */
	inline no (const std::string& message_ = "")
		: _errn (errno) 
		, _message (message_)
	{
		_message += ::strerror (_errn);
	}

	//! \brief Destructor
	inline virtual ~no () throw () {}
//@}

	//! \brief Virtual what
	inline
	virtual
	const char* what () const throw ()
	{
		return _message.c_str();
	}

	/** @brief Get number of error
		@return Integer error number
	 */
	inline
	operator int () const throw ()
	{
		return _errn;
	}

private:
	std::string _message;
	int _errn;

}; //.class err

} //::err

#endif //!_ERR_NO_H
