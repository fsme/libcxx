//@(#) The phonebook streambuf example
// Licence: LGPLv3
// Author: Andrew Wingorodov <http://andr.mobi/>
// $Id: addr_insert.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef __DB_ADDR_INSERT_H
#define __DB_ADDR_INSERT_H 1

#include <dbstream.h>
#include <berkeley.h>

namespace db {

/** @class addrbuf
	@brief Table IO stream buffer
*/
class addrbuf
	: public std::streambuf
{
typedef int int_type;
typedef std::streampos pos_type;

public:
	///\brief Constructor
	addrbuf () : std::streambuf(), addrbook () {}

	///\brief Destructor
	~addrbuf () { close(); }

///\name Open and Close
//@{
	///\brief Open streambuf
	void open (
	  	  const std::string path_ ///\param path_ Path to file
	);

	///\brief Close streambuf
	void close();
//@}

	///\brief Is open
	///\return True if open
	bool is_open () const { return addrbook.is_open(); }

	///\brief Input phone number
	void get_phone()
	{
		row.phone = ::strtoul ( (const char*)&obuffer,NULL,10);
		row.change = true;
		this->pbump (-count());
	}

	///\brief Input name 
	get_first ()
	{
		row.fname = std::string ( (const char*)&obuffer, count());
		row.change = true;
		this->pbump (-count());
	}

	///\brief Input last name 
	get_last ()
	{
		row.lname = std::string ( (const char*)&obuffer, count());
		row.change = true;
		this->pbump (-count());
	}

	///\brief Input email
	get_email ()
	{
		row.email = std::string ( (const char*)&obuffer, count());
		row.change = true;
		this->pbump (-count());
	}

protected:
///\name Base streambuf IO
//@{
	///\brief Output buffer sync
	///\return Zero if successful
	int_type sync ();

	///\brief Overflow output buffer
	///\return Last character or eof
	int_type overflow (
			int_type c ///\param c Last output character
		);
//@}

private:
	addrbuf (const addrbuf&);
	addrbuf& operator=(const addrbuf&);

	db::berkeley addrbook;

	///\brief Get count of output
	const std::streamsize count () const
	{
		return ( this->pptr() - this->pbase() );
	}

	struct _row {
		u_int64_t	phone; ///< Phone number, primary key
		std::string	fname; ///< First name
		std::string	lname; ///< Last name
		std::string	email; ///< Address line

		bool change;
		_row () : change (false) {}
	} row;

	union outbuf {
		u_int64_t	_phone;		///< Phone number, primary key
		const char	_fname [32]; ///< First name
		const char	_lname [32]; ///< Last name
		const char	_email [64]; ///< Address line
	} obuffer;

}; //.class addrbuf

/** @class addr_insert
	@brief Address book insert stream
*/
struct addr_insert
	: public db_ostream<addrbuf>
{
using db_ostream::db_ios;

	///\Constructor
	addr_insert (const char* path_)
		: db_ostream<addrbuf>(path_)
	{}

	addr_insert& operator<< ( addr_insert& (*function)(addr_insert&) )
	{
		return function(*this);
	}

///\name Manipulators
//@{
	///\brief Input phone number
	addr_insert& phone () { base_ios::rdbuf()->get_phone(); return *this; }

	///\brief Input name 
	addr_insert& first () { base_ios::rdbuf()->get_first(); return *this; } 

	///\brief Input name 
	addr_insert& last ()  { base_ios::rdbuf()->get_last(); return *this; }

	///\brief Input name 
	addr_insert& email () { base_ios::rdbuf()->get_email(); return *this; }
//@}

	///\brief Call base
	template <class T>
	addr_insert& operator<<(T arg)
	{
		db_ostream::operator<<(arg);
		return *this;
	}

}; //.addr_insert

//
inline addr_insert& phone (addr_insert& s) { return s.phone(); }
inline addr_insert& first (addr_insert& s) { return s.first(); }
inline addr_insert& last  (addr_insert& s) { return s.last(); }
inline addr_insert& email (addr_insert& s) { return s.email(); }


} //::db

#endif //!__DB_ADDR_INSERT_H
