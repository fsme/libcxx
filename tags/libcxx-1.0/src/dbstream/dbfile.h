//@(#) Abstract DB file (open, close, type, access method, etc.)
//@(#) Licence: LGPLv3
//@(#) Author: Andrew Wingorodov <http://andr.ru/>
//$Id: dbfile.h,v 1.1.1.1 2007-12-20 15:53:44 wingorodov Exp $

#ifndef __DB_FILE_H
#define __DB_FILE_H 1

#include <string>

namespace db {

#pragma options enum=1;
///\brief Type of database
enum dbtype {
	  unknown///< Without type, empty superclass
	, bdb	///< Berkeley DB (version 4.4.20 is implemented)
///< to do
//	, gdbm	///< GNU dbm
//	, myisam///< MyISAM (by MySQL)
//	, innodb///< Inno DB
//	, dbm	///< UNIX dbm
//	, ndbm	///< UNIX new dbm
///< to do

};

/** @class file
	@brief Database file
*/
class file
{
public:
///\name Constructor and Destructor
//@{
	///\brief By default
	file (dbtype type_ = db::unknown)
		: _is_open (false)
		, _type (type_)
	{}

	///\brief Destructor
	virtual ~file () = 0;
//@}

///\name Open and Close
//@{
	///\brief Open database's file
	virtual
	void open (
			  const std::string& path_ ///\param path_ Path to DB file
			, ios::openmode mode_ ///\param mode_ I/O mode (read/write/update)
		) = 0;

	///\brief Close DB file
	virtual
	void close () = 0;
//@}

	///\brief Get type of database
	///\return Type of table of database
	bool is_open () const { return _is_open; }

	///\brief Get type of database
	///\return Type of table of database
	bool is_type (const dbtype type_) const { return (_type == type_); }

protected:
	dbtype _type;
	bool _is_open;

	///\brief DB environment create
	virtual
	void init_db_env () = 0;

	///\brief DB environment destroy
	virtual
	void drop_db_env () = 0;

private:
	file (const file&);
	file& operator=(const file&);

}; //.class file

inline file::~file () {}
} //::db

#endif //!__DB_FILE_H
