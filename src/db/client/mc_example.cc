#include <iron.h>
#include <msg_stats.h>
#include <mc_client.hpp>

using namespace std;
using namespace sql;

struct sender {
	u_int32_t addr;
	u_int16_t amount;
};

int main( int argc, char** argv )
{
try {
	env::iron.configure ("./default.conf");
	if( argc < 3 )
		throw invalid_argument ("Usage: mc_example key data");

	memcachecli <sender> mc (3); ///3 sec. to expiries

	sql::insert<> ( argv [2], strlen (argv[2]));
	cout << mc.data().str() << endl;

} catch (const std::exception& e) {
		cout << e.what() << endl;
	}
  return 0;
}
