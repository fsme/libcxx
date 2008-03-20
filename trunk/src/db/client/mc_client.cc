//@(#) memcachecli (code)
// Authors: Dmitry Parshin, Andrew Wingorodov <http://andr.mobi/> 
//$Id$

#include <mc_client.hpp>

namespace sql
{

//
  bool erase( const std::string& strKey )
  {
	return ( 0 == ::mc_delete (
				  McConn::it().GetConn()
				, (char*)strKey.data()
				, strKey.size(), 0 ) ) ? true : false;
  }

//
  void* get( const std::string& strKey,  size_t& size_)
  {
	return ::mc_aget2 (
			  McConn::it().GetConn()
			, (char*)strKey.data()
			, strKey.size()
			, &size_ );
  }

}//namespace sql
