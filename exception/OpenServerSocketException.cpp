#include "OpenServerSocketException.hpp"

const char *OpenServerSocketException::what(void) const throw()
{
    return "OpenServerSocketException";
}
