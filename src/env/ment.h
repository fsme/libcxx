//@(#) Superclass for environment
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: ment.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef _ENV_MENT_H
#define _ENV_MENT_H 1

#include <env.h>

#include <map>
#include <string>
#include <stdexcept>

#include <err.h>
#include <logostream.h>

using namespace log;

namespace env {

/** @class ironment
	@brief Superclass for global variables and user's environment
*/
template <class key_type, class value_type>
class ironment
{
public:
//! \name Constructor and Destructor
//@{
	//! \brief By default
	inline ironment ()
		: vars ()
	{}

	//! \brief Destructor
	inline virtual ~ironment () {}
//@}

/// \name User environment
//@{
	/// \brief Set environment
	///	\throw If ::setenv(3) is failed (return nonzero)
	void setenv (
	  const std::string& name_ /// \param name_ Name of variable
	, const std::string& newvalue_ /// \param newvalue_ New value of variable
	, const bool repl_ = true /// \param repl_ Replace existing variable
	) const
	{
		if (::setenv (name_.c_str(), newvalue_.c_str(), (repl_) ? 1 : 0)
			!= 0 ) throw err::no ("setenv: ");
	}
	
	/** @brief Get environment
		@param name_ Name of variable
		@return Value of the name_
	 */
	const
	std::string& getenv (const std::string& name_) const
	{
		static std::string _name;
		const char* s;
		if ( (s = ::getenv (name_.c_str()) ) == NULL)
			throw std::invalid_argument (name_ + ": not found");

		return _name = s;
	}
//@}

//! \name Access
//@{
	/** @brief Get value by name
		@param key_ Name of variable
		@return Value of the key
	 */
	inline
	value_type& operator [] (const key_type& key_)
	{
		return vars [key_];
	}

	/** @brief Try find variable in environment by name
		@param key_ Name of variable
		@return True if exist
	 */
	inline
	bool
 	 exists (const key_type& key_) const
	{
		return vars.find (key_) != vars.end ();
	}
	
	/** @brief Get value by name readonly
		@param key_ Name of variable
		@return Value of the key
		@throw If key_ not found
	 */
	inline
	const value_type&
	 operator () (const key_type& key_) const throw ()
	{
		std::map<key_type, value_type>::const_iterator i;
		if ( (i = vars.find (key_)) != vars.end () )
			return i->second;
		//else
			throw std::invalid_argument ("key not found");
	}
//@}

//! \name Virtual interface for configuration
//@{
	/** @brief Configure environment from configuration file
		@param path_ File config
	 */
	virtual
	void
	 configure (const std::string& path_) = 0;
//@}

	/** @brief Print environment to std::ostream
		@param sep_ Separator between key -> value ( ':' by default ) 
		@param ostrm Output stream (log::s by default)
	 */
	inline
	void
	 print ( const std::string sep_ = ":",  std::ostream& ostrm = s )
	{
		std::map<key_type,value_type>::const_iterator
		i = vars.begin ();
		for (; i != vars.end (); ++i)
			ostrm << i->first << sep_ << i->second << std::endl;
	}

protected:
	std::map<key_type,value_type> vars; ///< Map of environment of variables


}; //.class ironment

} //::env

#endif //!_ENV_MENT_H
