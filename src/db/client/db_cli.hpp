#ifndef DBCLI_H_
#define DBCLI_H_

#ifdef HAVE_CONFIG_H
   #include <config.h>
#endif

#ifdef LINUX
#include <db_185.h>
#else
#include <db.h>
#endif //LINUX

#include <sys/types.h>
#include <fcntl.h>
#include <stdexcept>


namespace sql {

///\class db_cli
///\brief Berkeley DB client
template <typename DATA>
class db_cli
{
    
  std::auto_ptr<DATA> _data;
  std::string _key;

  DB* m_pDb;

public:

  db_cli( const std::string& strDbFileName, bool bCreate = false )
  {
    open_db_file( strDbFileName, bCreate );
  }


  virtual ~db_cli(){};

  ///\brief Get statistic by key
  void select(
	      const std::string& key_ ///\param key_ Key of selecting value
	      )
  {
    if( m_pDb == NULL )
      {
	clear();
	return;
      }
    DBT dbtKey, dbtData;
    dbtKey.data = (void*)key_.data();
    dbtKey.size = key_.size(); 
    ::memset( &dbtData, 0, sizeof( DBT ) );
    dbtData.size = sizeof( DATA );
    if( 0 != m_pDb->get( m_pDb, &dbtKey, &dbtData, 0 ) ||sizeof(DATA) != dbtData.size )
      {
	clear();
	return;
      }
    _data = std::auto_ptr<DATA>( new DATA );
    *_data = *(DATA*)dbtData.data;
    _key = key_;
  }

  ///\brief Clear current element value
  void clear()
  {
    _key.clear();
    _data.reset();
  }

  ///\brief Is current value exists in database
  bool exists() const
  {
    return !_key.empty();
  }

  ///\brief Get data reference
  ///\return Message's statistic reference
  DATA& data () const 
  { 
    if( !exists() )
      throw std::runtime_error( "data" );
    return *_data.get(); 
  }


  /**
   * \brief function insert
   * This function insert DATA record to db
   * input parameters:
   * @strKey - record key 
   * @pStats - pointer to record value
   */
  void insert( const std::string& strKey, const DATA* pStats )
  {
    if( NULL != m_pDb )
      {
	DBT dbtKey, dbtData;
	dbtKey.data = (void*)strKey.data();
	dbtKey.size = strKey.size();
	dbtData.data = (void*)pStats;
	dbtData.size = sizeof(DATA);
	if( 0 != m_pDb->put( m_pDb, &dbtKey, &dbtData, 0 ) )
	  throw std::runtime_error( "Error during inserting in BDB" );

	//TODO:inefective when inserting more than one record
	m_pDb->sync( m_pDb, 0 );
      }else
      	  throw std::runtime_error( "Error during inserting in BDB" );
    
  }

private:

  void open_db_file( const std::string& strFileName, bool bCreate )
  {
    int iFlag = bCreate ? O_RDWR|O_CREAT : O_RDWR;
    int iMode = bCreate ? 432 : 0;
    if( NULL == ( m_pDb = ::dbopen( strFileName.c_str(), iFlag, iMode , DB_HASH, NULL ) ) )
      throw std::runtime_error( "Db file open error" );
  }

};

} //::sql

#endif //DBCLI_H_
