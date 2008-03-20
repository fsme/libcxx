//@(#) A memcached memcachecli (header)
// Author: Andrew Wingorodov <http://andr.mobi/> , Dmitry Parshin
//$Id: mc_client.hpp 574 2008-03-12 10:39:07Z wingorodov $

#ifndef MC_CLIENT_H
#define MC_CLIENT_H 1

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <memcache.h>

#include <logostream.hpp>
#include <singleton.hpp>
#include <iron.h>

using namespace std;
using namespace cxx;

namespace sql {

/** 
* \brief class McConnection
* This class is used as singleton
* it serves connection to memcached
*/
class McConnection
{
public:

McConnection():m_bConnected( false )
{
	m_pMc = ::mc_new ();
	if (0 == m_pMc)
		throw std::runtime_error ("mc_new error");
}
    
~McConnection()
{
	::mc_free( m_pMc );
}
    
    
memcache* GetConn()
{
	if( !m_bConnected )
	{
	  cxx::logs << debug
				<< "MC::GetConn: mc_server = " << env::iron( "mc_server" )
				<< " mc_port = " << env::iron( "mc_port" ) << std::endl;
	  mconnect( 
		   env::iron( "mc_server" ),
		   env::iron( "mc_port" )
		    );
	  m_bConnected = true;
	}
      
      return m_pMc;
    }

private:
    
    struct memcache *m_pMc; ///< Memcache pointer
    
    bool m_bConnected;
    
///\brief Connect to single server
void mconnect (
	  const std::string& addr_ ///\param addr_ Network's address
	, const std::string& port_ = "11211" ///\param port_ Service's port
) {
	if ( 0 > ::mc_server_add ( m_pMc, addr_.c_str(), port_.c_str() ) )
		throw std::runtime_error ("mc::memcachecli->mconnect error");
}
  
///\brief Connect to cluster
void mconnect (
	   std::vector<std::string>& addr_///\param addr_ List of address,port
) {
   if (0 != addr_.size()%2) addr_.push_back ("11211");
      
	std::vector<std::string>::const_iterator i = addr_.begin();
	bool no_carrier = true;
	for (; i != addr_.end (); ++i)
	{
		if ( 0 > ::mc_server_add ( m_pMc, (*i).c_str(), (*(++i)).c_str() ) )
		no_carrier = true;
	}
	if (no_carrier)
		throw std::runtime_error ("mc::memcachecli->mconnect error");
}
    
};
  
typedef single::ton<McConnection> McConn;


///\brief Delete data from memcached
///\return Is data removed
///\param strKey Key of data
bool erase( const std::string& strKey );


///\brief Get generic data from memcached
///\return Pointer that must be deallocated with free()
void* get(
	  const std::string& strKey ///\param strKey Key of data
	, size_t& size_ ///\param size_ Size of data
);


///\brief Insert to memcached
template<typename T>
void insert (
	  const std::string& key_ ///\param key_ Key of data
	, const T& data_ ///\param data_ Data
	, time_t expr_ = 0 ///\param expr_ Time to expire of data
) {
  if( 0 != ::mc_set(\
		  McConn::it().GetConn()
		, (char*)key_.data()
		, key_.size()
		, &data_
		, sizeof(T)
		, expr_ 
		, 0 )
  )
	throw std::runtime_error( "Error during inserting in MC" );
}

///\class memcachecli
///\brief Client of memcached
  
template< typename DATA >
class memcachecli
{
typedef std::basic_string< char,std::char_traits<char> > string_type;

std::auto_ptr<DATA> _data;
size_t _size;

string_type _key;
time_t _expr;
    
public:
    
///\brief Create memcache client
explicit memcachecli (
	  const std::string& key_ ///\param key_ Select if defined
	, const time_t expr_ = 0 ///\param expr_ Time to expire
)
	: mc (0)
	, _key (key_)
	, _expr (expr_)
{
	mc = McConn::it().GetConn();
	if ( 0 < _key.size()) select (_key);
}

///\brief Create memcache client
memcachecli (
	const time_t expr_ = 0 ///\param expr_ Time to expire
)
	: mc (0)
	, _expr (expr_)
{
	mc = McConn::it().GetConn();
}

///\brief Destructor
virtual ~memcachecli ()
{}
	
///\brief Get statistic by key
void select(
	const string_type& key_ ///\param key_ Key of selecting value
) {
	DATA* pStats= (DATA*) get ( key_, _size );

	if (0 != pStats)
	{
		_key = key_;
		_data = std::auto_ptr<DATA>( new DATA);
		data() = *pStats;
		::free( pStats );
		return;
	}
	clear();
}

///\brief Set time to expire
void expire (
	const time_t expr_ ///\param expr_ Time to expire for mc_set
) {
	_expr = expr_;
}
    
///\brief Rehash (insert & update) statistic
void rehash (
		const string_type& key_///\param key_ Key of data
) {
	_key = key_;

	DATA* ptr = (DATA*) get ( key_ , _size );

	if (0 != ptr) ///< Gotcha!
	{
		_data = (std::auto_ptr<DATA>) new DATA;
		data() = *ptr;
		::free (ptr);
		++data().hits;
		update ();
	  
	} else { ///< Not found
		_data = (std::auto_ptr<DATA>) new DATA;

		data().reset();
		data().hits = 1;
		data().touch();
		  
		insert ( _key, *_data.get(), _expr);
	}
}

///\brief Size of data
const size_t& size () const { return _size; }

///\brief Clear current element value
void clear()
{
	_data.reset();
}
    
///\brief Remove current key
void erase ()
{
	if ( exists() )
	{
	  sql::erase( _key );
	  clear();
	}
}

///\brief Is current value exists in database
bool exists() const
{
	return !( NULL == _data.get() );
}
    
///\brief Get data reference
///\return Message's statistic reference
DATA& data () const 
{ 
	if( !exists() )
		throw std::runtime_error("memcachecli::data: no exists");

	return *_data.get(); 
}
    
///\brief Set current statistic to database
///\return
bool update () const
{
	insert ( _key, *_data.get(), _expr );
	return true;
}

private:

    struct memcache *mc; ///< Memcache pointer
};

}//::sql


#endif
