//@(#) Database IO stream header
// Licence: LGPLv3
// Author: Andrew Wingorodov <http://andr.mobi/>
// $Id: dbstream.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef __DB_STREAM_H
#define __DB_STREAM_H 1

#include <streambuf.h>
#include <iosfwd>
#include <string>

namespace db {

/** @class db_ios
	@brief Databases I/O system
*/
template <class DBstreambuf>
class db_ios
	: virtual public std::ios
{
protected:
//@{
	///\Constructor by default
	db_ios ()
		: std::ios (0)
		, dbuf ()
	{}

	///\brief Constructor with path to file
	explicit db_ios (
		  const char* path_ /// \param path_ Path to database file
	)
		: std::ios (0) { open (path_); }

	///\brief Destructor
	virtual ~db_ios () { this->close(); }
//@}

	DBstreambuf dbuf; ///< Stream buffer

public:
	///\brief Open DB IOS
	void open (
		  const char* path_ ///\param path_ To file 
	) {
		dbuf.open (path_);
		this->init (&dbuf);
	}

	///\brief Close DB IOS
	void close () { dbuf.close (); }

	///\brief Is open
	///\return True if streambuf is open
	bool is_open () const { return dbuf.is_open (); }

	///\brief Get stream buffer
	///\return Pointer to stream buffer
	DBstreambuf* rdbuf () { return &dbuf; }
};

/** @class db_ostream 
	@brief DB output stream
*/
template <class DBstreambuf>
class db_ostream
	: public std::ostream
	, public db_ios<DBstreambuf>
{
typedef std::ostream ostream_type;
typedef db_ios<DBstreambuf> base_ios;

	using base_ios::dbuf;

public:
	///\brief Constructor by default
	db_ostream ()
		: ostream_type(0)
		, base_ios()
	{}

	///\brief Constructor with path to output file
	explicit db_ostream (
		  const char* path_ ///\param path_ Path to database
		)
		: ostream_type(0)
		, base_ios (path_)
	{}

	///\brief Virtual destructor
	virtual ~db_ostream ()
	{}

}; //.db_ostream

/** @class db_istream 
	@brief DB input stream
*/
template <class DBstreambuf>
class db_istream
	: public std::istream
	, public db_ios<DBstreambuf>
{
typedef std::istream istream_type;
typedef db_ios<DBstreambuf> base_ios;

	using base_ios::dbuf;

public:
	///\brief Constructor by default
	db_istream ()
		: istream_type(0)
		, base_ios()
	{}

	///\brief Constructor with path to output file
	explicit db_istream (
		  const char* path_ ///\param path_ Path to database
		)
		: istream_type(0)
		, base_ios (path_)
	{}

	///\brief Virtual destructor
	virtual ~db_istream ()
	{}
};

} //::db

#endif //!__DB_STREAM_H
