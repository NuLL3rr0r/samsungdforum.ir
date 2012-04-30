#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "exception.hpp"

using namespace std;
using namespace boost;
using namespace SamsungDForumIr;

Exception::Exception(const string &msg)
    : runtime_error(msg)
{
}


