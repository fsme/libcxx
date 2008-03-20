//@(#) MySQL mysqlcli (header)
// Author: Andrew Wingorodov <http://andr.mobi/>, Dmitry Parshin
// Licence: LGPLv3
//$Id: mysqlcli.hpp 265 2008-02-12 08:23:12Z wingorodov $

#ifndef _SQL_MYSQLCLI_H
#define _SQL_MYSQLCLI_H 1

#include <mysql/mysql.h>

#include <vector>
#include <string>
#include <stdexcept>
#include <singleton.hpp>
#include <iron.h>

using namespace std;

namespace sql {

///\class MySQLConnection
///\brief MySQL connection manager

  
//TODO: now this class can serve only one physical connection (connection to one mysql server)
  class MySqlConnection
  {
  public:
    
    MySqlConnection()
		: m_iRefCount(0)
		, m_pConn (0)
    {}
    
    MYSQL* GetConn()
    {
      if( 0 == m_iRefCount )
	{
	  mysqlcli_init();
	  connect( 
		  env::iron( "mysql_server" ),
		  env::iron( "mysql_user" ),
		  env::iron( "mysql_pass" ),
		  env::iron( "mysql_db_name" )
		   );
	}else if( 0 != ::mysql_ping( m_pConn ) )
		  throw std::runtime_error( "Connection with MySql server is broken" );
      ++m_iRefCount;
      return m_pConn;
    }
    
    void CloseConn()
    {
      if( 0 == m_iRefCount )
	return;
      if( 0 == --m_iRefCount )
	close();
    }
    
  private:
    
    int m_iRefCount;
    
    MYSQL *m_pConn;
    
    ///\brief Init
    void mysqlcli_init ()
    {
      m_pConn = ::mysql_init (m_pConn);
      if (0 == m_pConn)
	throw std::runtime_error ("mysql_init error");
    }
    
    
    ///\brief Connect to MySQL server
    void connect (
		  const std::string& host_ ///\param host_ Server address
		  , const std::string& user_ ///\param user_ User login
		  , const std::string& pass_ ///\param pass_ Password
		  , const std::string& dbname_ ///\param dbname_ Databases name 
		  ) {
      if ( 0 > ::mysql_real_connect (			\
				     m_pConn
				     , host_.c_str()
				     , user_.c_str()
				     , pass_.c_str()
				     , dbname_.c_str()
				     , 0 , 0 , 0 )
	   ) throw std::runtime_error (::mysql_error (m_pConn));
    }
    
    ///\brief Close connection
    void close ()
    {
      if (m_pConn) ::mysql_close (m_pConn);
      m_pConn = 0;
    }
  };
  
  typedef single::ton<MySqlConnection> MySqlConn;
  
///\class mysqlcli
///\brief Client MySQL
  
class mysqlcli
{
protected:
    MYSQL* conn;
    
public:
    
    ///\brief Constructor with connect to single server
    mysqlcli ()
      : conn (0)
    {
      conn = MySqlConn::it().GetConn();
    }
    
    ///\brief Destructor
    virtual ~mysqlcli () 
    { 
      MySqlConn::it().CloseConn();
    }
    
    ///\brief Insert virtual interface
    virtual long query (
			const std::string& query_ ///\param query_ Query string
			) = 0;
    
    ///\brief Escape string
    ///\return Real escape string reference
    const std::string&
    escape_string (\
		   const std::string& from_ ///\param from_ String of escape
	) const {
      static std::string _to;
      char* to_ = new char [ (from_.size()<<1)+1 ];
      ::mysql_real_escape_string ( conn, to_, from_.data(), from_.size());
      _to = to_;
      delete [] to_;
      return _to;
    }

    ///\brief Escape string from const char[]
    ///\return Real escape string reference
    const std::string&
    escape_string (\
		  const char* from_ ///\param from_ Data for escaped
		, size_t len_ ///\param len_ Size of data of from
	) const {
      static std::string _to;
      char* to_ = new char [ (len_<<1)+1 ];
      ::mysql_real_escape_string ( conn, to_, from_, len_);
      _to = to_;
      delete [] to_;
      return _to;
    }
};
  
}//::sql

#endif ///_SQL_MYSQLCLI_H
