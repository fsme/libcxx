//@(#) Berkeley DB file implementation
//@(#) Licence: LGPLv3
//@(#) Author: Andrew Wingorodov <http://andr.ru/>
//$Id: berkeley.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef __DB_BERKELEY_H
#define __DB_BERKELEY_H 1

#include <db44/db.h>

#include <stdexcept>

#include <logostream.h>
#include <dbfile.h>

namespace db {

/** @class berkeley
	@brief Berkeley DB file implementation
*/
class berkeley
	: public file
{
public:
///\name Constructor and Destructor 
//@{
	///\brief Construtor by default
	berkeley (
		 const DBTYPE type_ = DB_UNKNOWN ///\param type_ Access method
	)
		: file (db::bdb)
		, _type (type_)
		, _flags ()
	{
		init_db_env ();

		int ret;
		ret = ::db_create (&_dbp, _dbenv, 0);
		if (0 != ret)
			throw std::runtime_error (db_strerror (ret));
	}

	///\brief Destructor
	virtual ~berkeley ()
	{
		close ();
		drop_db_env();
	}
//@}

///\name Open and close
//@{
	///\brief Open database's file
	virtual
		void open (
		  const std::string& path_ ///\param path_ Path to DB file
		, const ios::openmode mode_ ///\param mode_ I/O mode
	);

	///\brief Close DB file
	virtual void close ();
//@}

protected:
	DB*      _dbp;		///< Database
	DB_ENV*  _dbenv;	///< Database environment
	DB_TXN   _txnid;	///< Transaction
	DBTYPE   _type;		///< Type of access

	u_int32_t	_flags;

	///\brief _db_env initialization
	void init_db_env ();

	///\brief _db_env destruction
	void drop_db_env ();

private:
	berkeley (const berkeley&);
	berkeley & operator=(const berkeley&);
};

//
inline
void
 berkeley::init_db_env ()
{
_dbenv = 0;
/**
	ret = ::db_env_create (&_dbenv, 0);
	if (0 != ret)
		throw std::runtime_error (db_strerror (ret));

	ret = _dbenv->open (_dbenv, "/tmp/db", 0, 0);
	if (0 != ret)
		throw std::runtime_error (db_strerror (ret));
**/
}

//
inline
 void berkeley::drop_db_env ()
{
_dbenv = 0;
/**
	int ret = _dbenv->close (_dbenv, 0);
	if (0 != ret)
		log::s <<  "berkeley::close::DB_ENV->open error: "
			   << db_strerror (ret) << std::endl;

	ret = _dbenv->remove(_dbenv, NULL, 0);
		log::s <<  "berkeley::close::DB_ENV->remove error: "
			   << db_strerror (ret) << std::endl;
**/
}

} //::db

#endif //!__DB_BERKELEY_H
