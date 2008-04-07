
#include <msg_stats.h>
#include <db_cli.hpp>
#include <iostream>

using namespace std;
using namespace sql;

int main( int argc, char** argv )
{
  try{
    db_cli<msgstats> db( "db_test.db", true );
    
    msgstats st;
	st.reset();
    st.hits = 123;

    db.insert( "asd", &st );
    db.clear();

    db.select( "asd" );
    if( !db.exists() )
      std::cout << "Don`t exists" << std::endl;
    else
      {
	std::cout << "Exists" << std::endl;
	std::cout << "hits = " << db.data().hits << std::endl;
      }
  }catch( std::exception& e )
    {
      std::cout << "exception: " << e.what() << std::endl;
    }
  return 0;
}
