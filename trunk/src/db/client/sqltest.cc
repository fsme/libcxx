#include <map>
#include <string>
#include <iostream>
#include <selquery.hpp>

int main()
{
	sql::selquery q;
	q.query( "SELECT hash,flags FROM message" );
	std::map<std::string,std::string> mapRes;
	for(;;)
	{
		q.FetchRow( mapRes );
		if( mapRes.empty() )
			break;
		for( std::map< std::string, std::string >::iterator It = mapRes.begin(); It != mapRes.end(); It++ )
			std::cout << "Field name: " << It->first << " Field value: " << It->second << std::endl;
		std::cout << "ROW END" << std::endl;
	}
	return 0;
}
