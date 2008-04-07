//@(#) MRAs database selquery (header)
// Author:  Dmitry Parshin

#ifndef _SQL_SELECTQUERY_H
#define _SQL_SELECTQUERY_H 

#include <mysqlcli.hpp>
#include <map>
#include <string>

using namespace std;

namespace sql {

  class CSqlQueryException: public std::runtime_error
  {
  public:
	CSqlQueryException( const std::string& strDescr, unsigned int iErrno ):std::runtime_error( strDescr )
																		 ,m_iErrno( iErrno )
	{}

	unsigned int Errno() { return m_iErrno; }
  private:

	unsigned int m_iErrno;
  };


  ///\class selquery
  ///\brief Client MySQL
  
  class selquery: public mysqlcli
  {
  public:
    ///\brief Constructor by default
    selquery ():m_pRes(0)
	{}
    
    ///\brief Destructor
    virtual ~selquery ()
	{
	  FreeRes();
	}
    
    bool FetchRow( std::map<std::string,std::string>& mapRow )
    {
      mapRow.clear();

	  if( NULL == m_pRes )
		return false;

      MYSQL_ROW Row;
      if( Row = ::mysql_fetch_row( m_pRes ) )
		{
		  for( int i = 0; i < m_vecFieldNames.size(); i++ )
			mapRow[ m_vecFieldNames[i] ] = ( NULL == Row[i] )? "" : Row[i];
		}
	  else
		return false;

      return true;
    }
    
    unsigned long long FetchRowCount()
    {
      return ::mysql_num_rows( m_pRes );
    }
    
    ///\brief Insert virtual interface
    virtual long query (
						const std::string& query_ ///\param query_  Query string
						) 
    {
	  FreeRes();
      cxx::logs << debug << "Executing SQL query: " << query_ << std::endl;
      
      int ret= ::mysql_real_query ( conn, query_.data(), query_.size());
      if (0 != ret)
		throw CSqlQueryException( ::mysql_error(conn), ::mysql_errno(conn) );

      m_pRes = ::mysql_store_result( conn );
      m_vecFieldNames.clear();
      if( m_pRes )
		{
		  unsigned int num_fields = mysql_num_fields( m_pRes );
		  MYSQL_FIELD *fields = mysql_fetch_fields( m_pRes );
		  for( int i = 0; i < num_fields; i++ )
			m_vecFieldNames.push_back( fields[i].name );
		}

      long lRes;
      if( -1 == ( lRes = ::mysql_affected_rows( conn ) ) )
		throw std::runtime_error ( "Query error" );

      return lRes;			
    }
    
  private:

	void FreeRes()
	{
	  if( m_pRes )
		{
		  mysql_free_result( m_pRes );
		  m_pRes = NULL;
		}
	}
    
    MYSQL_RES *m_pRes;
    
    std::vector< std::string > m_vecFieldNames;
    
  };
  
}//::sql

#endif ///_SQL_SELECTQUERY_H
