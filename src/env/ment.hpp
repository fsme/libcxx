//@(#) Superclass for environment
//@(#) Licence: LGPLv3
//@(#) Author Andrew Wingorodov <http://andr.ru/>
//$Id: ment.hpp 343 2008-02-15 16:28:15Z parshin $

#ifndef _ENV_MENT_H
#define _ENV_MENT_H 1

#include <map>
#include <string>
#include <stdexcept>

#include <er.h>
#include <logostream.hpp>

using namespace cxx;

namespace env {

///\class ironment
///\brief Superclass for global variables and user's environment

template <typename Key, typename Value>
class ironment
{
public:
///\name Constructor and Destructor
//@{
	///\brief By default
	ironment () : vars () {}

	///\brief Destructor
	virtual ~ironment () {}
//@}

///\name User environment
//@{
	///\brief Set environment
	///\throw If ::setenv(3) is failed (return nonzero)

	void setenv (
	  const std::string& name_ ///\param name_ Name of variable
	, const std::string& newvalue_ ///\param newvalue_ New value of variable
	, const bool repl_ = true ///\param repl_ Replace existing variable
	) const {
		if (::setenv (name_.c_str(), newvalue_.c_str(), (repl_) ? 1 : 0)
			!= 0 ) throw er::no ("setenv: ");
	}
	
	///\brief Get environment
	///\return Value of the name_
	const
	std::string& getenv (
		const std::string& name_ ///\param name_ Name of variable
	) {
		const char* s;
		if ( (s = ::getenv (name_.c_str()) ) == NULL)
			throw std::invalid_argument (name_ + ": not found");

		vars [name_] = s;
		return vars [name_];
	}
//@}

///\name Access
//@{
	///\brief Get value by name
	///\return Value of the key

	Value& operator [] (
		const Key& key_ ///\param key_ Name of variable
	) {
		return vars [key_];
	}

	///\brief Try find variable in environment by name
	///\return True if exist

	bool exists (
		const Key& key_ ///\param key_ Name of variable
	) const {
		return vars.find (key_) != vars.end ();
	}
	
	///\brief Get value by name readonly
	///\return Value of the key
	///\throw If key_ not found

	const Value& operator () (
		const Key& key_ ///\param key_ Name of variable
	) const {
		typename std::map<Key, Value>::const_iterator i;
		if ( (i = vars.find (key_)) != vars.end () )
			return i->second;
		//else
			throw std::invalid_argument ("key " + key_ + " not found");
	}
//@}

///\name Virtual interface for configuration
//@{
	///\brief Configure environment from configuration file
	virtual
	void configure (
		const std::string& path_///\param path_ File config
	) = 0;
//@}

	///\brief Print environment to std::ostream
	void
	 print (
		  const std::string sep_ = ":" ///\param sep_ Separator 
		, std::ostream& ostrm = logs ///\param ostrm Output stream
	) {
		typename std::map<Key,Value>::const_iterator i = vars.begin ();
		for (; i != vars.end (); ++i)
			ostrm << i->first << sep_ << i->second << std::endl;
	}

protected:
	std::map<Key,Value> vars; ///< Map of environment of variables

}; //.class ironment

} //::env

#endif //!_ENV_MENT_H
