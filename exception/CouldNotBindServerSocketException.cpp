#include "CouldNotBindServerSocketException.hpp"

const char *CouldNotBindServerSocketException::what(void) const throw()
{
    return "CouldNotBindServerSocketException";
}
