#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP


#include <string>
#include <stdexcept>
#include <boost/exception/all.hpp>

namespace SamsungDForumIr {
    class Exception;
}

class SamsungDForumIr::Exception : public std::runtime_error
{
public:
    Exception(const std::string &msg);
};


#endif /* EXCEPTION_HPP */


