#include "NonBlockServerSocketException.hpp"

const char *NonBlockServerSocketException::what(void) const throw()
{
    return "NonBlockServerSocketException";
}
