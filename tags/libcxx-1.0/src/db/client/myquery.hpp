//@(#) MRAs database myquery (header)
// Author: Andrew Wingorodov <http://andr.mobi/>
// Licence: LGPLv3
//$Id: myquery.hpp 519 2008-03-05 16:01:27Z parshin $

#ifndef _SQL_MYQUERY_H
#define _SQL_MYQUERY_H 1

#include <mysqlcli.hpp>
#include <string>

using namespace std;

namespace sql {

  ///\class myquery
  ///\brief Client MySQL
  
  class myquery: public mysqlcli
  {
  public:
   
    ///\brief Constructor with connect to single server
    explicit myquery ()
    {}
    
    ///\brief Destructor
    virtual ~myquery () {}
    
    ///\brief Insert virtual interface
    virtual long query (
			const std::string& query_ ///\param query_  Query string
	) {
      logs << info << " myquery::query.size(" << query_.size() << ")" << endl;
      logs << debug << " myquery::QUERY:" << endl << query_ << endl;

      int ret= ::mysql_real_query ( conn, query_.data(), query_.size());
      if (0 != ret)
		throw std::runtime_error ( ::mysql_error (conn) );
      
      long lRes;
      if( -1 == ( lRes = ::mysql_affected_rows( conn ) ) )
		throw std::runtime_error ( "Query error" );
      
      logs << info << "myquery::query: OK" << endl;
      return lRes;			
	}
};
  
}//::sql

#endif ///_SQL_MYQUERY_H
